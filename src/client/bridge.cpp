#include "bridge.h"

#include <systems/ecs.h>
#include <entity_wrapper.h>
#include <systems/logger.h>
#include <systems/resource_manager.h>

#include <components/transform.h>
#include <components/hierarchy.h>
#include <components/camera.h>
#include <components/material.h>

#include <resources/model.h>
#include <resources/material.h>

unsigned int bridge::CreateEntity() {
    return (unsigned int)ECS::Create().Id();
}

void bridge::AddTransform(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    entity.GetOrAdd<Components::Transform>();
}

void bridge::AddHierarchy(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    entity.GetOrAdd<Components::Hierarchy>();
}

void bridge::AddCamera(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    entity.GetOrAdd<Components::PerspectiveCamera>();
}

float bridge::GetPositionX(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    auto pos = entity.Get<Components::Transform>().Position();
    return pos.x;
}

float bridge::GetPositionY(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    auto pos = entity.Get<Components::Transform>().Position();
    return pos.y;
}

float bridge::GetPositionZ(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    auto pos = entity.Get<Components::Transform>().Position();
    return pos.z;
}
void bridge::SetPosition(unsigned int id, float x, float y, float z) {
    auto entity = Entity((entt::entity)id);
    entity.Get<Components::Transform>().SetPosition(glm::vec3(x, y, z));
}

void bridge::HierarchyAddChild(unsigned int id, unsigned int child) {
    auto entity = Entity((entt::entity)id);
    entity.Get<Components::Hierarchy>().AddChild(Entity((entt::entity)child));
}

rust::Vec<unsigned int> bridge::HierarchyGetChildren(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    rust::Vec<unsigned int> children;
    entity.Get<Components::Hierarchy>().ForEachChild([&children](Entity child) {
        children.push_back((unsigned int)child.Id());
    });
    return children;
}

rust::Vec<unsigned int> bridge::HierarchyGetDescendants(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    rust::Vec<unsigned int> descendants;
    entity.Get<Components::Hierarchy>().ForEachDescendant([&descendants](Entity descendant) {
        descendants.push_back((unsigned int)descendant.Id());
    });
    return descendants;
}

void bridge::CameraSetActive(unsigned int id) {
    auto entity = Entity((entt::entity)id);
    Renderer::Instance().SetActiveCamera(entity);
}

void bridge::InstantiateModel(unsigned int id, rust::Str modelPath) {
    auto entity = Entity((entt::entity)id);
    auto& model = ResourceManager::Get<Model>((std::string)modelPath);
    model.Instantiate(entity);
}

void bridge::AddMaterial(unsigned int id, rust::Str materialName) {
    auto entity = Entity((entt::entity)id);
    entity.AddWithArgs<Components::Material>(ResourceInfo<Material>((std::string)materialName).Identifier());
}

void bridge::CreateMaterial(rust::Str materialName, rust::Str vertPath, rust::Str fragPath) {
    auto shaderId = ResourceManager::Load(ResourceInfo<Shader>((std::string)vertPath, (std::string)fragPath));
    ResourceManager::Emplace(ResourceInfo<Material>((std::string)materialName), Material(shaderId));
}

void bridge::LoadModel(rust::Str modelPath) {
    ResourceManager::Load<Model>((std::string)modelPath);
}