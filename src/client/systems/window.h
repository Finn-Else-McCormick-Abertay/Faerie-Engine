#pragma once

#include <systems/system_lifecycle_declare.h>

#include <SDL.h>

#include <string>
#include <memory>
#include <maths_types.h>

class Window final {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(Window)
public:
    static int2 WindowSize();
    static float WindowAspect();

private:
    // Begin main loop. Do not call except from main
    static void MainLoop();
    friend int main(int argc, char *argv[]);
    
    void Update();

    SDL_Window* p_window = nullptr;
    bool m_shouldClose = false;

    int2 m_windowSize;
};