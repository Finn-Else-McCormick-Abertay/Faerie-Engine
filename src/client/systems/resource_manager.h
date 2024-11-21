#pragma once

#include <systems/system.h>

#include <resources/resource_info.h>
#include <resources/shader.h>
#include <resources/texture.h>
#include <resources/script.h>
#include <resources/model.h>
#include <resources/mesh.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstddef>

#include <vfspp/VirtualFileSystem.hpp>

// Must be initialised after the window
class ResourceManager final : public ISystem
{
public:
    static ResourceManager& Instance();
    
    template<typename T> static T& Get(ResourceIdentifier id) {
        return __Map<T>().at(id);
    }
    template<typename T> static T& Get(const ResourceInfo<T>& info) { return Get<T>(info.Identifier()); }
    template<typename T> static T& Get(const std::string& path) { return Get<T>(ResourceInfo<T>(path)); }


    template<typename T> static ResourceIdentifier Load(const ResourceInfo<T>& info) {
        __Map<T>().emplace(info.Identifier(), __LoadInternal<T>(info));
        return info.Identifier();
    }
    template<typename T> static ResourceIdentifier Load(const std::string& path) { return Load<T>(ResourceInfo<T>(path)); }
    

    template<typename T> static void Unload(ResourceIdentifier id) {
        __Map<T>().erase(id);
    }
    template<typename T> static void Unload(const ResourceInfo<T>& info) { Unload<T>(info.Identifier()); }
    template<typename T> static void Unload(const std::string& path) { return Unload<T>(ResourceInfo<T>(path)); }

protected:
    // Get the relevent map for a given resource type
    template<typename T> static std::unordered_map<ResourceIdentifier, T>& __Map();
    
    // Load resource from given resource definition
    template<typename T> static T __LoadInternal(const ResourceInfo<T>&);

protected:
    static vfspp::VirtualFileSystem& FileSystem();
    
    static bool FileExists(const std::string& virtualPath);
    static std::string ReadTextFile(const std::string& virtualPath);
    static std::vector<uint8_t> ReadBinaryFile(const std::string& virtualPath);

private:
    ResourceManager() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    std::unique_ptr<vfspp::VirtualFileSystem> pm_vfs;

    std::unordered_map<ResourceIdentifier, Texture> m_textures;
    std::unordered_map<ResourceIdentifier, Shader> m_shaders;
    std::unordered_map<ResourceIdentifier, Script> m_scripts;
    std::unordered_map<ResourceIdentifier, faerie::Mesh> m_meshes;
    std::unordered_map<ResourceIdentifier, Model> m_models;
};

template<> std::unordered_map<ResourceIdentifier, Texture>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Shader>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Script>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, faerie::Mesh>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Model>& ResourceManager::__Map();

// __LoadInternal defined in cpp file of relevant resource

template<> Texture ResourceManager::__LoadInternal(const ResourceInfo<Texture>&);
template<> Shader ResourceManager::__LoadInternal(const ResourceInfo<Shader>&);
template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>&);
template<> Model ResourceManager::__LoadInternal(const ResourceInfo<Model>&);