#pragma once

#include <systems/render_system.h>

#include <SDL.h>

#include <string>
#include <memory>

class WindowSystem : public ISystem
{
public:
    void Update();

    bool ShouldClose();

    IRenderSystem* RenderSystem();

private:
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    SDL_Window* p_window = nullptr;
    bool m_shouldClose = false;
    std::string m_appPath;

    std::unique_ptr<IRenderSystem> m_renderSystem;
};