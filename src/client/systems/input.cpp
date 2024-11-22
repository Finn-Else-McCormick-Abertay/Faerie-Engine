#include "input.h"

Input& Input::Instance() {
    static Input input;
    return input;
}

bool Input::InitImpl() {
    return true;
}

void Input::ShutdownImpl() {}

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
            SDL_GameControllerAxis axisX, axisY;
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

}


void Input::SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) {

}

void Input::SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) {
    
}


void Input::SendMouseButtonEvent(MouseButton button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) {

}

void Input::SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) {

}

void Input::SendMouseWheelEvent(vec2 scroll, Uint32 id) {

}