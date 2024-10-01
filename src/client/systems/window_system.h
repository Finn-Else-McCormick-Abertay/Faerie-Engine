#pragma once

#include <systems/subsystem.h>
#include <systems/render_system.h>
#include <systems/input_system.h>

class IWindowSystem : public ISubsystem
{
public:
    virtual void Update() = 0;

    virtual IRenderSystem* RenderSystem() = 0;
    virtual IInputSystem* InputSystem() = 0;

    virtual bool ShouldClose() = 0;
};