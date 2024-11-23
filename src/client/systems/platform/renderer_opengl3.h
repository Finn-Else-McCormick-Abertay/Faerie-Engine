#pragma once

#include <systems/renderer.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>

class RendererOpenGl3 : public Renderer {
    FAERIE___SYSTEM_POLYMORPHIC_LIFECYCLE_DECLARE(RendererOpenGl3, Renderer)
public:
    virtual void CreateContext(SDL_Window* window) override;
    virtual SDL_WindowFlags AdditionalWindowFlags() override;

    virtual void Render() override;

protected:
    virtual void BeginImGuiFrame() override;

private:
    SDL_Window* p_window = nullptr;
    SDL_GLContext m_glContext;
    std::string m_glslVersion;

    void DrawGameScene();
};