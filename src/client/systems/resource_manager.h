#pragma once

#include <systems/system.h>

#include <string>
#include <unordered_map>
#include <memory>

#include <resources/shader.h>
#include <resources/texture.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>

// Must be initialised after the window
class ResourceManager : public ISystem
{
public:
    static ResourceManager& Instance();

    template<typename T, typename...Args>
    static T Load(Args...);

    template<typename T, typename...Args>
    static T Get(Args...);

private:
    ResourceManager() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    std::string m_appPath;

    std::unordered_map<std::string, Texture> m_textures;
    std::unordered_map<std::string, Shader> m_shaders;
};


template<> Texture ResourceManager::Load<Texture>(const std::string&);
template<> Texture ResourceManager::Get<Texture>(const std::string&);

template<> Shader ResourceManager::Load<Shader>(const std::string& vert, const std::string& frag);
template<> Shader ResourceManager::Get<Shader>(const std::string& vert, const std::string& frag);