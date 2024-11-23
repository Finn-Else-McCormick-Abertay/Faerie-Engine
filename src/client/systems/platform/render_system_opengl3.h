#pragma once

#include <systems/render_system.h>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <string>

class RenderSystemOpenGl3 : public RenderSystem {
    FAERIE___SYSTEM_POLYMORPHIC_LIFECYCLE_DECLARE(RenderSystemOpenGl3, RenderSystem)
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