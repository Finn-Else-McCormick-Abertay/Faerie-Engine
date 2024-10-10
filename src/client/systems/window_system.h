#pragma once

#include <systems/render_system.h>

#include <SDL.h>

#include <string>
#include <memory>

class WindowSystem : public ISystem
{
public:
    virtual bool Init() override;
    virtual void Shutdown() override;

    void Update();

    bool ShouldClose();

    IRenderSystem* RenderSystem();

private:
    SDL_Window* p_window = nullptr;
    bool m_shouldClose = false;
    std::string m_appPath;

    std::unique_ptr<IRenderSystem> m_renderSystem;
};