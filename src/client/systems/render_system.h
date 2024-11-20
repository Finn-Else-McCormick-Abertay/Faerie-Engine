#pragma once

#include <systems/system.h>
#include <entity_wrapper.h>

#include <SDL.h>

class IRenderSystem : public ISystem
{
public:
    virtual void Render() = 0;
    void ImGuiRender();

    virtual void CreateContext(SDL_Window* window) = 0;

    Entity ActiveCamera() const;
    void SetActiveCamera(Entity);

protected:
    virtual void BeginImGuiFrame() = 0;

    Entity m_cameraEntity;
};