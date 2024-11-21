#include "material.h"

#include <systems/ecs.h>
#include <systems/resource_manager.h>
#include <systems/logger.h>

#include <resources/material.h>

Components::Material::Material(ResourceIdentifier material) : m_materialId(material) {}

ResourceIdentifier Components::Material::MaterialId() const { return m_materialId; }

void Components::Material::__OnAdded(Entity ent) {
    if (!m_materialId) { return; }
    auto& material = ResourceManager::Get<::Material>(m_materialId);
    ECS::Registry().emplace<ShaderHandle>(ent.Id());
    auto&& shaderStorage = ECS::Registry().storage<ShaderHandle>(material.ShaderId());
    shaderStorage.push(ent.Id());
}

void Components::Material::__OnRemoved(Entity ent) {
    if (!m_materialId) { return; }
    auto& material = ResourceManager::Get<::Material>(m_materialId);
    ECS::Registry().erase<ShaderHandle>(ent.Id());
    auto&& shaderStorage = ECS::Registry().storage<ShaderHandle>(material.ShaderId());
    shaderStorage.erase(ent.Id());
}