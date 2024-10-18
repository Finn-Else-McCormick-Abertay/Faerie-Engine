#pragma once

#include <systems/system.h>

#include <resources/resource_info.h>
#include <resources/shader.h>
#include <resources/texture.h>

#include <string>
#include <unordered_map>
#include <memory>

#include <vfspp/VFS.h>

// Must be initialised after the window
class ResourceManager : public ISystem
{
public:
    static ResourceManager& Instance();

    template<typename T>
    static ResourceIdentifier Load(const ResourceInfo<T>&);

    template<typename T>
    static T Get(ResourceIdentifier);

    template<typename T>
    static T Get(const ResourceInfo<T>& info) {
        return Get<T>(info.Identifier());
    }

private:
    ResourceManager() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    std::unique_ptr<vfspp::VirtualFileSystem> pm_virtualFileSystem;

    std::unordered_map<ResourceIdentifier, Texture> m_textures;
    std::unordered_map<ResourceIdentifier, Shader> m_shaders;
};

template<> ResourceIdentifier ResourceManager::Load<Texture>(const ResourceInfo<Texture>&);
template<> Texture ResourceManager::Get<Texture>(ResourceIdentifier);

template<> ResourceIdentifier ResourceManager::Load<Shader>(const ResourceInfo<Shader>&);
template<> Shader ResourceManager::Get<Shader>(ResourceIdentifier);