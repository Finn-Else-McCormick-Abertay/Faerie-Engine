#include "window_system.h"

#ifdef OPENGL3
#include <platform/render_system_opengl3.h>
#endif // OPENGL3

#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>

bool WindowSystem::InitImpl() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError()); 
        return false;
    }

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

#ifdef OPENGL3
    m_renderSystem = std::make_unique<RenderSystemOpenGl3>();
    windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | windowFlags);
#endif // OPENGL3

    if (!m_renderSystem->Init()) {
        return false;
    }

    p_window = SDL_CreateWindow("Untitled Tabletop Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
    if (p_window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return false;
    }

    m_renderSystem->CreateContext(p_window);

    return true;
}

void WindowSystem::ShutdownImpl() {
    if (m_renderSystem) { m_renderSystem->Shutdown(); }
    if (p_window) { SDL_DestroyWindow(p_window); }
    SDL_Quit();
}

void WindowSystem::Update() {
    ImGuiIO& io = ImGui::GetIO();
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            m_shouldClose = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(p_window))
            m_shouldClose = true;

        if (!io.WantCaptureKeyboard) {
            if (event.type == SDL_KEYDOWN) {

            }
            if (event.type == SDL_KEYUP) {

            }
            if (event.type == SDL_TEXTINPUT) {

            }
        }

        if (!io.WantCaptureMouse) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {

            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                
            }
            if (event.type == SDL_MOUSEMOTION) {
                
            }
            if (event.type == SDL_MOUSEWHEEL) {
                
            }
        }
    }
    if (SDL_GetWindowFlags(p_window) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        return;
    }

    m_renderSystem->ImGuiRender();
    m_renderSystem->Render();
}

bool WindowSystem::ShouldClose() {
    return m_shouldClose;
}

IRenderSystem* WindowSystem::RenderSystem() {
    return m_renderSystem.get();
}