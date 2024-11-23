#pragma once

#include <systems/system_lifecycle_declare.h>
#include <entity_wrapper.h>

#include <SDL.h>

class Renderer {
    FAERIE___SYSTEM_ABSTRACT_SINGLETON_LIFECYCLE_DECLARE(Renderer)
public:
    virtual void Render() = 0;
    void ImGuiRender();

    virtual void CreateContext(SDL_Window* window) = 0;
    virtual SDL_WindowFlags AdditionalWindowFlags() = 0;

    Entity ActiveCamera() const;
    void SetActiveCamera(Entity);

protected:
    virtual void BeginImGuiFrame() = 0;

    Entity m_cameraEntity;
};