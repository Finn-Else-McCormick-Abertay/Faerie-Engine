#pragma once

#include <systems/input_system.h>

class InputSystemImplSDL2 : public IInputSystem
{
public:
    virtual bool Init() override;
    virtual void Shutdown() override;
};