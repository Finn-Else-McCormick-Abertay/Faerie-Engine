#pragma once

#include <string>
#include <vector>
#include <memory>
#include <maths_types.h>

#include <resources/resource_info.h>
#include <resources/mesh.h>

#include <entity_wrapper.h>

class Model
{
public:
    Entity Instantiate(Entity parent = Entity()) const;

private:
    struct Node {
        std::string name;
        vec3 position; quat rotation; vec3 scale;
        std::vector<std::unique_ptr<Node>> children;
        std::vector<ResourceInfo<faerie::Mesh>> meshes;
    };
    Node m_rootNode;

    friend class ResourceManager;
};