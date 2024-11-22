#pragma once

#include <systems/system.h>

#include <SDL.h>
#include <maths_types.h>

// Interface for logic which should be exposed to the window but nothing else
class IInputSystemInternal
{
protected:
    friend class WindowSystem;

    virtual void SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) = 0;

    virtual void SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) = 0;
    virtual void SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) = 0;

    virtual void SendMouseButtonEvent(Uint8 button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) = 0;
    virtual void SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) = 0;
    virtual void SendMouseWheelEvent(vec2 scroll, Uint32 id) = 0;
};

class Input final : public ISystem, public IInputSystemInternal
{
public:
    static Input& Instance();

private:
    Input() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    virtual void SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) override;
    
    virtual void SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) override;
    virtual void SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) override;

    virtual void SendMouseButtonEvent(Uint8 button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) override;
    virtual void SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) override;
    virtual void SendMouseWheelEvent(vec2 scroll, Uint32 id) override;
};