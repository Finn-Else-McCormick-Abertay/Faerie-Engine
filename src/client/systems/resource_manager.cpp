#include "resource_manager.h"

#include <SDL.h>

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::InitImpl() {
    char* pathBuf = SDL_GetBasePath();
    m_appPath = std::string(pathBuf);
    SDL_free(pathBuf);

    return true;
}

void ResourceManager::ShutdownImpl() {
}


template<>
Texture ResourceManager::Get<Texture>(const std::string& path) {
    return Instance().m_textures.at(path);
}

template<>
Texture ResourceManager::Load<Texture>(const std::string& path) {
    auto& inst = Instance();

    Texture texture;

    inst.m_textures.insert(std::make_pair(path, texture));
    return inst.m_textures.at(path);
}


template<>
Shader ResourceManager::Get<Shader>(const std::string& vertPath, const std::string& fragPath) {
    std::string hashPath = "v_" + vertPath + ":f_" + fragPath;
    return Instance().m_shaders.at(hashPath);
}

template<>
Shader ResourceManager::Load<Shader>(const std::string& vert, const std::string& frag) {
    auto& inst = Instance();
    std::string hashPath = "v_" + vert + ":f_" + frag;

    std::string vertPath = inst.m_appPath + vert, fragPath = inst.m_appPath + frag;
    
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertPath.c_str(), std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	} else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertPath.c_str());
		getchar();
		return Shader();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragPath.c_str(), std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertPath.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragPath.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    Shader shader;
    shader.ProgramId = ProgramID;

    inst.m_shaders.insert(std::make_pair(hashPath, shader));

    return inst.m_shaders.at(hashPath);
}