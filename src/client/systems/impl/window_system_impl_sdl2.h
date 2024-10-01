#pragma once

#include <systems/window_system.h>
#include <systems/impl/render_system_impl_opengl3.h>
#include <systems/impl/input_system_impl_sdl2.h>

#include <SDL.h>

#include <string>
#include <memory>

class WindowSystemImplSDL2 : public IWindowSystem
{
public:
    virtual bool Init() override;
    virtual void Shutdown() override;

    virtual void Update() override;

    virtual bool ShouldClose() override;

    virtual IRenderSystem* RenderSystem() override;
    virtual IInputSystem* InputSystem() override;

private:
    SDL_Window* p_window = nullptr;
    bool m_shouldClose = false;
    std::string m_appPath;

    std::unique_ptr<RenderSystemImplOpenGl3> m_renderSystem;
    std::unique_ptr<InputSystemImplSDL2> m_inputSystem;
};