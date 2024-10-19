#include "texture.h"

#include <systems/resource_manager.h>

template<>
Texture ResourceManager::Get<Texture>(ResourceIdentifier id) {
    return Instance().m_textures.at(id);
}

template<>
ResourceIdentifier ResourceManager::Load<Texture>(const ResourceInfo<Texture>& info) {
    auto& inst = Instance();

    Texture texture;

	ResourceIdentifier id = info.Identifier();
    inst.m_textures.emplace(id, texture);
    return id;
}