#pragma once

#include <systems/system_lifecycle_declare.h>

#include <SDL.h>
#include <maths_types.h>
#include <input_action.h>

#include <map>

// Interface for logic which should be exposed to the window but nothing else
class IInputSystemInternal
{
protected:
    friend class WindowSystem;

    virtual void SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) = 0;

    virtual void SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) = 0;
    virtual void SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) = 0;

    virtual void SendMouseButtonEvent(MouseButton button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) = 0;
    virtual void SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) = 0;
    virtual void SendMouseWheelEvent(vec2 scroll, Uint32 id) = 0;
};

class Input final : public IInputSystemInternal {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(Input)
public:
    static const ActionBinding& GetBinding(const ActionIdentifier&);
    static void SetBinding(const ActionIdentifier&, ActionBinding&&);
    static void SetBinding(const ActionIdentifier&, const ActionBinding&);

private:
    virtual void SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) override;
    
    virtual void SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) override;
    virtual void SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) override;

    virtual void SendMouseButtonEvent(MouseButton button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) override;
    virtual void SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) override;
    virtual void SendMouseWheelEvent(vec2 scroll, Uint32 id) override;

    std::map<ActionIdentifier, ActionBinding> m_bindings;

    void UpdateBindingReflectionMaps();
    std::map<SDL_Scancode, std::set<ActionIdentifier>> m_scancodesToActions;
    std::map<SDL_GameControllerButton, std::set<ActionIdentifier>> m_controllerButtonsToActions;
    std::map<MouseButton, std::set<ActionIdentifier>> m_mouseButtonsToActions;
    std::map<SDL_GameControllerAxis, std::set<ActionIdentifier>> m_controllerAxesToActions;
    std::map<MouseMotionSource, std::set<ActionIdentifier>> m_mouseMotionSourcesToActions;
};