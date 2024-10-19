#include "texture.h"

#include <systems/resource_manager.h>

template<>
Texture ResourceManager::__LoadInternal(const ResourceInfo<Texture>& info) {
    return Texture();
}

template<>
void ResourceManager::__UnloadInternal(Texture& texture) {
    // Clean up texture resource
}