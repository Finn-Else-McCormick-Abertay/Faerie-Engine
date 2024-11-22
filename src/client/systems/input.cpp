#include "input.h"

Input& Input::Instance() {
    static Input input;
    return input;
}

bool Input::InitImpl() {
    return true;
}

void Input::ShutdownImpl() {}


void Input::SendKeyEvent(SDL_Scancode scanCode, bool isPressed, bool isRepeat, Uint16 modifierState) {

}


void Input::SendControllerButtonEvent(SDL_GameControllerButton button, bool isPressed, SDL_JoystickID id) {

}

void Input::SendControllerAxisEvent(SDL_GameControllerAxis axis, float value, SDL_JoystickID id) {
    
}


void Input::SendMouseButtonEvent(Uint8 button, bool isPressed, bool isDoubleClick, int2 screenPos, Uint32 id) {

}

void Input::SendMouseMotionEvent(int2 screenPos, int2 motion, Uint32 id) {

}

void Input::SendMouseWheelEvent(vec2 scroll, Uint32 id) {

}