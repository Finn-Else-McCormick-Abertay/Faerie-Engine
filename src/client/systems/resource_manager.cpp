#include "resource_manager.h"
#include <systems/system_lifecycle_define.h>

#include <SDL.h>

// ZipFileSystem includes miniz-cpp, which is a bit skittish.
// It needs cstring to be included before it for some reason, and it will cause multiple-definition errors if
// included in more than one cpp file, so we can only interact with it from outside via the virtual file system.
#include <cstring>
#include <vfspp/ZipFileSystem.hpp>
#include <vfspp/NativeFileSystem.hpp>

#include <sstream>
#include <vector>

#include <systems/Logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ResourceManager)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ResourceManager)

bool ResourceManager::__Internal_Init() {
	std::string appPath;
	{
		char* pathBuf = SDL_GetBasePath();
		appPath = std::string(pathBuf);
		SDL_free(pathBuf);
	}

	pm_vfs = std::make_unique<vfspp::VirtualFileSystem>();

	auto rootFs = std::make_unique<vfspp::NativeFileSystem>(appPath);
	rootFs->Initialize();

	pm_vfs->AddFileSystem("/", std::move(rootFs));

	//Logger::Debug(*this, "Root path: ", appPath);
    return true;
}

void ResourceManager::__Internal_Shutdown() {}

template<> std::unordered_map<ResourceIdentifier, Texture>& ResourceManager::__Map() { return Instance().m_textures; }
template<> std::unordered_map<ResourceIdentifier, Shader>& ResourceManager::__Map() { return Instance().m_shaders; }
template<> std::unordered_map<ResourceIdentifier, Script>& ResourceManager::__Map() { return Instance().m_scripts; }
template<> std::unordered_map<ResourceIdentifier, faerie::Mesh>& ResourceManager::__Map() { return Instance().m_meshes; }
template<> std::unordered_map<ResourceIdentifier, Model>& ResourceManager::__Map() { return Instance().m_models; }
template<> std::unordered_map<ResourceIdentifier, Material>& ResourceManager::__Map() { return Instance().m_materials; }

vfspp::VirtualFileSystem& ResourceManager::FileSystem() { return *Instance().pm_vfs; }

bool ResourceManager::FileExists(const std::string& path) {
	auto file = FileSystem().OpenFile(vfspp::FileInfo(path), vfspp::IFile::FileMode::Read);
	return file != nullptr;
}

std::string ResourceManager::ReadTextFile(const std::string& path) {
	if (auto file = FileSystem().OpenFile(vfspp::FileInfo(path), vfspp::IFile::FileMode::Read); file && file->IsOpened()) {
		std::stringstream sstr;
		file->Read(sstr, file->Size());
		file->Close();
		return sstr.str();
	}
	Logger::Error<ResourceManager>("Failed to open text file ", path);
	return "";
}

std::vector<uint8_t> ResourceManager::ReadBinaryFile(const std::string& path) {
	if (auto file = FileSystem().OpenFile(vfspp::FileInfo(path), vfspp::IFile::FileMode::Read); file && file->IsOpened()) {
		std::vector<uint8_t> vec; vec.reserve(file->Size());
		file->Read(vec, file->Size());
		file->Close();
		return vec;
	}
	Logger::Error<ResourceManager>("Failed to open binary file ", path);
	return std::vector<uint8_t>();
}