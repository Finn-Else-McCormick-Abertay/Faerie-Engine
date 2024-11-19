#include "shader.h"

#include <util/hash.h>
#include <systems/resource_manager.h>
#include <systems/logger.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

Shader::Shader(unsigned int programId) : m_programId(programId) {}

unsigned int Shader::ProgramId() { return m_programId; }


ResourceInfo<Shader>::ResourceInfo(const std::string& vertPath, const std::string& fragPath) : m_vert(vertPath), m_frag(fragPath) {}

ResourceIdentifier ResourceInfo<Shader>::Identifier() const {
    size_t hash = 0;
    util::hash_combine(hash, typeid(Shader).hash_code(), m_vert, m_frag);
    return hash;
}

const std::string& ResourceInfo<Shader>::VertPath() const { return m_vert; }
const std::string& ResourceInfo<Shader>::FragPath() const { return m_frag; }


template<>
Shader ResourceManager::__LoadInternal(const ResourceInfo<Shader>& info) {
	auto glStatus = [](GLuint id, GLenum name, GLenum type = GL_SHADER){
		GLint result;
		switch (type) {
			case GL_SHADER:  { glGetShaderiv(id, name, &result); }  break;
			case GL_PROGRAM: { glGetProgramiv(id, name, &result); } break;
			default: return false;
		}
		return (bool)result;
	};

	auto glOutputError = [&info](GLuint id, GLenum type = GL_SHADER) {
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
			Logger::Error("Shader", info, ": ", errorMessage.data());
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
	compile(vertShaderId, ReadTextFile(info.VertPath()));

	GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compile(fragShaderId, ReadTextFile(info.FragPath()));

	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertShaderId);
	glAttachShader(programId, fragShaderId);
	glLinkProgram(programId);

	if (!glStatus(programId, GL_LINK_STATUS, GL_PROGRAM)) {
		glOutputError(programId, GL_PROGRAM);
		Logger::Error<Shader>("Failed to load from ", info, ": shader program does not compile.");
		// Maybe do something if fail, like replace with default shader or throw error? idk
	}
	
	glDetachShader(programId, vertShaderId);
	glDetachShader(programId, fragShaderId);
	
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

    return Shader(programId);
}

template<>
void ResourceManager::__UnloadInternal(Shader& shader) {
    glDeleteProgram(shader.ProgramId());
}