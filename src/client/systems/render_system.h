#pragma once

#include <systems/system.h>

#include <SDL.h>

class IRenderSystem : public ISystem
{
public:
    SYSTEM_LOGGER_NAME(RenderSystem)

    virtual void Render() = 0;
    void ImGuiRender();

    virtual void CreateContext(SDL_Window* window) = 0;

protected:
    virtual void BeginImGuiFrame() = 0;
};