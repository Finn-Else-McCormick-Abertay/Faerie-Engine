#pragma once

#include <SDL.h>
#include <string>
#include <set>

class ActionIdentifier
{
public:
    ActionIdentifier(const std::string& ns, const std::string& action);

    std::string Namespace() const;
    std::string Action() const;

    std::string AsString() const;
    inline explicit operator std::string() const { return AsString(); }

    inline auto operator <=>(const ActionIdentifier& rhs) const { return AsString() <=> rhs.AsString(); }

private:
    std::string m_namespace, m_action;
};


using PhysicalKeyCode = SDL_Scancode;
using LogicalKeyCode = SDL_KeyCode;
using ControllerButton = SDL_GameControllerButton;
enum class MouseButton {
    LEFT = SDL_BUTTON_LEFT,
    MIDDLE = SDL_BUTTON_MIDDLE,
    RIGHT = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2
};

enum class MouseMotionSource {
    MOUSE_MOTION,
    MOUSE_WHEEL,
    //TOUCH,
    //EMULATED,
};
enum class ControllerStick { LEFT, RIGHT };
enum class ControllerTrigger { LEFT, RIGHT };

class ActionBinding
{
public:
    ActionBinding() = default;

    void Bind(PhysicalKeyCode);     void Unbind(PhysicalKeyCode);
    void Bind(LogicalKeyCode);      void Unbind(LogicalKeyCode);
    void Bind(ControllerButton);    void Unbind(ControllerButton);
    void Bind(MouseButton);         void Unbind(MouseButton);
    void Bind(MouseMotionSource);   void Unbind(MouseMotionSource);
    void Bind(ControllerStick);     void Unbind(ControllerStick);
    void Bind(ControllerTrigger);   void Unbind(ControllerTrigger);

    bool Has(PhysicalKeyCode);
    bool Has(LogicalKeyCode);
    bool Has(ControllerButton);
    bool Has(MouseButton);
    bool Has(MouseMotionSource);
    bool Has(ControllerStick);
    bool Has(ControllerTrigger);

private:
    friend class Input;

    // Button sources
    std::set<PhysicalKeyCode> m_physicalKeys;
    std::set<LogicalKeyCode> m_logicalKeys;
    std::set<ControllerButton> m_controllerButtons;
    std::set<MouseButton> m_mouseButtons;

    // Motion sources
    std::set<MouseMotionSource> m_mouseMotionSources;
    std::set<ControllerStick> m_controllerSticks;
    std::set<ControllerTrigger> m_controllerTriggers;
};