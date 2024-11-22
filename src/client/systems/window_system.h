#pragma once

#include <systems/render_system.h>

#include <SDL.h>

#include <string>
#include <memory>
#include <maths_types.h>

class WindowSystem final : public ISystem
{
public:
    static WindowSystem& Instance();

    IRenderSystem& RenderSystem();

    static int2 WindowSize();
    static float WindowAspect();

private:
    WindowSystem() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;
    
    // Begin main loop. Do not call except from main
    static void MainLoop();
    friend int main(int argc, char *argv[]);
    
    void Update();

    SDL_Window* p_window = nullptr;
    bool m_shouldClose = false;

    int2 m_windowSize;

    std::unique_ptr<IRenderSystem> m_renderSystem;
};