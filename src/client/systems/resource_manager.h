#pragma once

#include <systems/system.h>

#include <resources/resource_info.h>
#include <resources/shader.h>
#include <resources/texture.h>

#include <string>
#include <unordered_map>
#include <memory>

#include <vfspp/VirtualFileSystem.hpp>

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

    static vfspp::VirtualFileSystem& FileSystem();

    template<typename T>
    static T ReadFile(const std::string& virtualPath);

    std::unique_ptr<vfspp::VirtualFileSystem> pm_vfs;

    std::unordered_map<ResourceIdentifier, Texture> m_textures;
    std::unordered_map<ResourceIdentifier, Shader> m_shaders;
};

template<> std::string ResourceManager::ReadFile<std::string>(const std::string& virtualPath);
template<> const char* ResourceManager::ReadFile<const char*>(const std::string& virtualPath);

// Defined in resources/texture.cpp
template<> ResourceIdentifier ResourceManager::Load<Texture>(const ResourceInfo<Texture>&);
template<> Texture ResourceManager::Get<Texture>(ResourceIdentifier);

// Defined in resources/shader.cpp
template<> ResourceIdentifier ResourceManager::Load<Shader>(const ResourceInfo<Shader>&);
template<> Shader ResourceManager::Get<Shader>(ResourceIdentifier);