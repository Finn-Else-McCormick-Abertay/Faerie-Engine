#include "resource_manager.h"

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

// ZipFileSystem includes miniz-cpp, which is a bit skittish.
// It needs cstring to be included before it for some reason, and it will cause multiple-definition errors if
// included in more than one cpp file, so we can only interact with it from outside via the virtual file system.
#include <cstring>
#include <vfspp/ZipFileSystem.hpp>
#include <vfspp/NativeFileSystem.hpp>

#include <sstream>

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::InitImpl() {
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

    return true;
}

void ResourceManager::ShutdownImpl() {
	pm_vfs = nullptr;
}


vfspp::VirtualFileSystem& ResourceManager::FileSystem() { return *Instance().pm_vfs; }

template<>
std::string ResourceManager::ReadFile(const std::string& path) {
	if (auto file = FileSystem().OpenFile(vfspp::FileInfo(path), vfspp::IFile::FileMode::Read); file && file->IsOpened()) {
		std::stringstream sstr;
		file->Read(sstr, file->Size());
		return sstr.str();
	}
	return "";
}

template<>
const char* ResourceManager::ReadFile(const std::string& path) {
	return ReadFile<std::string>(path).c_str();
}


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


template<>
Shader ResourceManager::Get<Shader>(ResourceIdentifier id) {
    return Instance().m_shaders.at(id);
}

template<>
ResourceIdentifier ResourceManager::Load<Shader>(const ResourceInfo<Shader>& info) {
	auto glStatus = [](GLuint id, GLenum name, GLenum type = GL_SHADER){
		GLint result;
		switch (type) {
			case GL_SHADER:  { glGetShaderiv(id, name, &result); }  break;
			case GL_PROGRAM: { glGetProgramiv(id, name, &result); } break;
			default: return false;
		}
		return (bool)result;
	};

	auto glOutputError = [](GLuint id, GLenum type = GL_SHADER) {
		int logLength = 0;
		switch (type) {
			case GL_SHADER:  { glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength); }  break;
			case GL_PROGRAM: { glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength); } break;
		}
		if (logLength > 0) {
			std::vector<char> errorMessage(logLength+1);
			switch (type) {
				case GL_SHADER:  { glGetShaderInfoLog(id, logLength, NULL, &errorMessage[0]); }  break;
				case GL_PROGRAM: { glGetProgramInfoLog(id, logLength, NULL, &errorMessage[0]); } break;
			}
			printf("%s\n", &errorMessage[0]);
		}
	};
	
	auto compile = [&](GLuint id, const std::string& code){
		const char* sourcePtr = code.c_str();
		glShaderSource(id, 1, &sourcePtr, NULL);
		glCompileShader(id);

		glOutputError(id);
		return glStatus(id, GL_COMPILE_STATUS);
	};
    
	// Load and compile shaders
	GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	compile(vertShaderId, ReadFile<std::string>(info.VertPath()));

	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compile(fragShaderId, ReadFile<std::string>(info.FragPath()));

	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertShaderId);
	glAttachShader(programId, fragShaderId);
	glLinkProgram(programId);

	if (!glStatus(programId, GL_LINK_STATUS, GL_PROGRAM)) {
		glOutputError(programId, GL_PROGRAM);
		// Maybe do something if fail, like replace with default shader or throw error? idk
	}
	
	glDetachShader(programId, vertShaderId);
	glDetachShader(programId, fragShaderId);
	
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

	// Create resource object
	Shader shader = Shader(programId);
	ResourceIdentifier id = info.Identifier();

    Instance().m_shaders.emplace(id, shader);
    return id;
}