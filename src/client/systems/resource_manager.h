#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/resource_info.h>
#include <resources/shader.h>
#include <resources/texture.h>
#include <resources/script.h>
#include <resources/model.h>
#include <resources/mesh.h>
#include <resources/material.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstddef>
#include <functional>

#include <vfspp/VirtualFileSystem.hpp>

// Must be initialised after the window
class ResourceManager final {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ResourceManager)
public:    
    template<typename T> static T& Get(ResourceIdentifier id) {
        return __Map<T>().at(id);
    }
    template<typename T> static T& Get(const ResourceInfo<T>& info) { return Get<T>(info.Identifier()); }
    template<typename T> static T& Get(const std::string& path) { return Get<T>(ResourceInfo<T>(path)); }


    template<typename T> static ResourceIdentifier Load(const ResourceInfo<T>& info) {
        return Emplace(info, __LoadInternal<T>(info));
    }
    template<typename T> static ResourceIdentifier Load(const std::string& path) { return Load<T>(ResourceInfo<T>(path)); }
    

    template<typename T> static void Unload(ResourceIdentifier id) {
        __Map<T>().erase(id);
    }
    template<typename T> static void Unload(const ResourceInfo<T>& info) { Unload<T>(info.Identifier()); }
    template<typename T> static void Unload(const std::string& path) { return Unload<T>(ResourceInfo<T>(path)); }

    template<typename T> static void ForEach(std::function<void(ResourceIdentifier, const T&)> callback) {
        for (auto const& [id, resource] : __Map<T>()) {
            callback(id, resource);
        }
    }

    // !!SHOULD BE PRIVATE, IS ONLY PUBLIC TEMPORARILY FOR TESTING!!
    template<typename T> static ResourceIdentifier Emplace(ResourceIdentifier id, T&& resource) {
        __Map<T>().emplace(id, std::move(resource));
        return id;
    }
    template<typename T> static ResourceIdentifier Emplace(const ResourceInfo<T>& info, T&& resource) { return Emplace(info.Identifier(), std::move(resource)); }

protected:
    // Get the relevent map for a given resource type
    template<typename T> static std::unordered_map<ResourceIdentifier, T>& __Map();
    
    // Load resource from given resource definition
    template<typename T> static T __LoadInternal(const ResourceInfo<T>&);

    // !!SHOULD BE PRIVATE, IS ONLY PUBLIC TEMPORARILY FOR TESTING!!
public:
    static vfspp::VirtualFileSystem& FileSystem();
    
    static bool FileExists(const std::string& virtualPath);
    static std::string ReadTextFile(const std::string& virtualPath);
    static std::vector<uint8_t> ReadBinaryFile(const std::string& virtualPath);

private:
    std::unique_ptr<vfspp::VirtualFileSystem> pm_vfs;

    std::unordered_map<ResourceIdentifier, Texture> m_textures;
    std::unordered_map<ResourceIdentifier, Shader> m_shaders;
    std::unordered_map<ResourceIdentifier, Script> m_scripts;
    std::unordered_map<ResourceIdentifier, faerie::Mesh> m_meshes;
    std::unordered_map<ResourceIdentifier, Model> m_models;
    std::unordered_map<ResourceIdentifier, Material> m_materials;
};

template<> std::unordered_map<ResourceIdentifier, Texture>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Shader>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Script>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, faerie::Mesh>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Model>& ResourceManager::__Map();
template<> std::unordered_map<ResourceIdentifier, Material>& ResourceManager::__Map();

// __LoadInternal defined in cpp file of relevant resource

template<> Texture ResourceManager::__LoadInternal(const ResourceInfo<Texture>&);
template<> Shader ResourceManager::__LoadInternal(const ResourceInfo<Shader>&);
template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>&);
template<> Model ResourceManager::__LoadInternal(const ResourceInfo<Model>&);