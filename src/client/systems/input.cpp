#include "input.h"
#include <systems/system_lifecycle_define.h>
#include <systems/script_engine.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(Input)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(Input)

bool Input::__Internal_Init() {
    auto leftAction = ActionIdentifier("faerie", "left");
    auto leftBinding = GetBinding(leftAction);
    leftBinding.Bind(PhysicalKeyCode::SDL_SCANCODE_LEFT);
    SetBinding(leftAction, leftBinding);

    auto rightAction = ActionIdentifier("faerie", "right");
    auto rightBinding = GetBinding(rightAction);
    rightBinding.Bind(PhysicalKeyCode::SDL_SCANCODE_RIGHT);
    SetBinding(rightAction, rightBinding);
    
    auto upAction = ActionIdentifier("faerie", "up");
    auto upBinding = GetBinding(upAction);
    upBinding.Bind(PhysicalKeyCode::SDL_SCANCODE_UP);
    SetBinding(upAction, upBinding);
    
    auto downAction = ActionIdentifier("faerie", "down");
    auto downBinding = GetBinding(downAction);
    downBinding.Bind(PhysicalKeyCode::SDL_SCANCODE_DOWN);
    SetBinding(downAction, downBinding);

    return true;
}

void Input::__Internal_Shutdown() {}

const ActionBinding& Input::GetBinding(const ActionIdentifier& id) {
    // Will create a binding if none exists. Not sure if desirable but otherwise returning a const ref doesnt work
    return Instance().m_bindings[id];
}
void Input::SetBinding(const ActionIdentifier& id, ActionBinding&& binding) {
    Instance().m_bindings.insert_or_assign(id, std::move(binding));
    Instance().UpdateBindingReflectionMaps();
}
void Input::SetBinding(const ActionIdentifier& id, const ActionBinding& binding) {
    Instance().m_bindings.insert_or_assign(id, binding);
    Instance().UpdateBindingReflectionMaps();
}

void Input::UpdateBindingReflectionMaps() {
    m_scancodesToActions.clear();
    m_controllerButtonsToActions.clear();
    m_mouseButtonsToActions.clear();
    m_controllerAxesToActions.clear();
    m_mouseMotionSourcesToActions.clear();

    for (auto& [id, binding] : m_bindings) {
        for (auto physicalKey : binding.m_physicalKeys) { m_scancodesToActions[physicalKey].insert(id); }
        for (auto logicalKey : binding.m_logicalKeys) {
            auto physicalKey = SDL_GetScancodeFromKey(logicalKey);
            m_scancodesToActions[physicalKey].insert(id);
        }
        for (auto button : binding.m_controllerButtons) { m_controllerButtonsToActions[button].insert(id); }
        for (auto button : binding.m_mouseButtons) { m_mouseButtonsToActions[button].insert(id); }
        for (auto source : binding.m_mouseMotionSources) { m_mouseMotionSourcesToActions[source].insert(id); }
        for (auto stick : binding.m_controllerSticks) {
            switch (stick) {
                case ControllerStick::LEFT: {
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_LEFTX].insert(id);
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_LEFTY].insert(id);
                } break;
                case ControllerStick::RIGHT: {
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_RIGHTX].insert(id);
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_RIGHTY].insert(id);
                } break;
            }
        }
        for (auto trigger : binding.m_controllerTriggers) {
            switch (trigger) {
                case ControllerTrigger::LEFT: {
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_TRIGGERLEFT].insert(id);
                } break;
                case ControllerTrigger::RIGHT: {
                    m_controllerAxesToActions[SDL_CONTROLLER_AXIS_TRIGGERRIGHT].insert(id);
                } break;
            }
        }
    }
}




void Input::SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) {
    if (!m_scancodesToActions.contains(scanCode)) { return; }
    for (auto& action : m_scancodesToActions.at(scanCode)) {
        ScriptEngine::OnInput(action.AsString(), isPressed);
    }
}


void Input::SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) {
    if (!m_controllerButtonsToActions.contains(button)) { return; }
    for (auto& action : m_controllerButtonsToActions.at(button)) {
        ScriptEngine::OnInput(action.AsString(), isPressed);
    }
}

void Input::SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) {
    if (!m_controllerAxesToActions.contains(axis)) { return; }
    //for (auto& action : m_controllerAxesToActions.at(axis)) {
    //    ScriptEngine::OnInput(action.AsString(), isPressed);
    //}
}


void Input::SendMouseButtonEvent(MouseButton button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) {
    if (!m_mouseButtonsToActions.contains(button)) { return; }
    for (auto& action : m_mouseButtonsToActions.at(button)) {
        ScriptEngine::OnInput(action.AsString(), isPressed);
    }
}

void Input::SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) {
    if (!m_mouseMotionSourcesToActions.contains(MouseMotionSource::MOUSE_MOTION)) { return;}
    for (auto& action : m_mouseMotionSourcesToActions.at(MouseMotionSource::MOUSE_MOTION)) {
        ScriptEngine::OnInput(action.AsString(), vec2((float)motion.x, (float)motion.y));
    }
}

void Input::SendMouseWheelEvent(vec2 scroll, Uint32 id) {
    if (!m_mouseMotionSourcesToActions.contains(MouseMotionSource::MOUSE_WHEEL)) { return;}
    for (auto& action : m_mouseMotionSourcesToActions.at(MouseMotionSource::MOUSE_WHEEL)) {
        ScriptEngine::OnInput(action.AsString(), vec2((float)scroll.x, (float)scroll.y));
    }
}