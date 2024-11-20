#pragma once

#include <systems/render_system.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>

class RenderSystemOpenGl3 : public IRenderSystem
{
public:
    void CreateContext(SDL_Window* window);

    virtual void Render() override;

protected:
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    virtual void BeginImGuiFrame() override;

private:
    SDL_Window* p_window = nullptr;
    SDL_GLContext m_glContext;
    std::string m_glslVersion;

    void DrawGameScene();

    // TEMP! THESE SHOULDN'T BE HERE!
    GLuint vertexBuffer;
    GLuint vertexArrayId; 
};