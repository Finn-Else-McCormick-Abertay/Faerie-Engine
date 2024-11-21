#include "model.h"

#include <systems/resource_manager.h>
#include <systems/ecs.h>
#include <systems/logger.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <resources/mesh.h>

#include <components/hierarchy.h>
#include <components/transform.h>
#include <components/mesh.h>

template<> Model ResourceManager::__LoadInternal(const ResourceInfo<Model>& info) {
    auto data = ReadBinaryFile(info.Path());

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFileFromMemory(data.data(), data.size() * sizeof(uint8_t),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords);

    if (!scene) {
        Logger::Error<Model>("Assimp error : ", importer.GetErrorString());
        return Model();
    }

    auto meshResourceInfo = [&](unsigned int index){
        return ResourceInfo<faerie::Mesh>(info.Path() + "::" + std::to_string(index));
    };

    auto aiNodeToFaerieNode = [&](aiNode* inode){
        auto convertImpl = [&](aiNode* inode, auto& convertRef) -> Model::Node {
            Model::Node node;
            node.name = std::string(inode->mName.C_Str());

            aiVector3D position; aiQuaternion rot; aiVector3D scale;
            inode->mTransformation.Decompose(scale, rot, position);
            node.position = vec3(position.x, position.y, position.z);
            node.rotation = quat(rot.w, rot.x, rot.y, rot.z);
            node.scale = vec3(scale.x, scale.y, scale.z);

            for (unsigned int i = 0; i < inode->mNumMeshes; ++i) {
                node.meshes.push_back(meshResourceInfo(inode->mMeshes[i]));
            }

            for (unsigned int i = 0; i < inode->mNumChildren; ++i) {
                node.children.push_back(std::make_unique<Model::Node>(convertRef(inode->mChildren[i], convertRef)));
            }
            return node;
        };
        return convertImpl(inode, convertImpl);
    };

    Model model;
    model.m_rootNode = aiNodeToFaerieNode(scene->mRootNode);

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        
        aiMesh* mesh = scene->mMeshes[meshIndex];

        std::vector<faerie::Vertex> vertices; vertices.reserve(mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            faerie::Vertex vertex;
            vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0]) {
                vertex.texCoords = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            vertices.push_back(vertex);
        }
        
        std::vector<unsigned int> indices; indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
            aiFace& face = mesh->mFaces[faceIndex];
            for (unsigned int i = 0; i < face.mNumIndices; ++i) {
                indices.push_back(face.mIndices[i]);
            }
        }
        
        __Map<faerie::Mesh>().emplace(meshResourceInfo(meshIndex).Identifier(), faerie::Mesh(vertices, indices));
    }

    return model;
}

Entity Model::Instantiate(Entity parent) const {
    auto instantiateNode = [](const Model::Node& node){
        auto instantiateNodeImpl = [](const Model::Node& node, auto& instantiateNodeRef) -> Entity {  
            auto entity = ECS::Create();
            auto [hier, trans] = entity.Add<Components::Hierarchy, Components::Transform>();
            trans.SetPosition(node.position); trans.SetRotation(node.rotation); trans.SetScale(node.scale);
            if (node.meshes.size() > 1) {
                for (auto& resourceInfo : node.meshes) {
                    auto meshEntity = ECS::Create();
                    meshEntity.Add<Components::Hierarchy>().SetParent(entity);
                    meshEntity.Add<Components::Mesh>().meshId = resourceInfo.Identifier();
                }
            }
            else if (node.meshes.size() == 1) {
                entity.Add<Components::Mesh>().meshId = node.meshes.at(0).Identifier();
            }

            for (auto& childNode : node.children) {
                auto childEntity = instantiateNodeRef(*childNode, instantiateNodeRef);
                childEntity.template Get<Components::Hierarchy>().SetParent(entity);
            }
            return entity;
        };
        return instantiateNodeImpl(node, instantiateNodeImpl);
    };

    auto rootEntity = instantiateNode(m_rootNode);
    if (parent) { rootEntity.Get<Components::Hierarchy>().SetParent(parent); }

    return rootEntity;
}