#pragma once

#include <systems/system.h>

#include <SDL.h>

class IRenderSystem : public ISystem
{
public:
    virtual void Render() = 0;
    virtual void ImGuiRender() = 0;

    virtual void CreateContext(SDL_Window* window) = 0;
};