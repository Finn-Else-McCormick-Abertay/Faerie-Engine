#pragma once

#include <systems/subsystem.h>

class IRenderSystem : public ISubsystem
{
public:
    virtual void Render() = 0;
    virtual void ImGuiRender() = 0;
};