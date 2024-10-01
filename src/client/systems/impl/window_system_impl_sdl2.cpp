#include "window_system_impl_sdl2.h"

#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>

bool WindowSystemImplSDL2::Init() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError()); 
        return false;
    }

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
    
    char* pathBuf = SDL_GetBasePath();
    m_appPath = std::string(pathBuf);
    SDL_free(pathBuf);

    m_renderSystem = std::make_unique<RenderSystemImplOpenGl3>();
    if (!m_renderSystem->Init()) {
        return false;
    }

    m_inputSystem = std::make_unique<InputSystemImplSDL2>();
    if (!m_inputSystem->Init()) {
        return false;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    p_window = SDL_CreateWindow("Untitled Tabletop Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (p_window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return false;
    }

    m_renderSystem->CreateContext(p_window);

    return true;
}

void WindowSystemImplSDL2::Shutdown() {
    if (m_renderSystem) { m_renderSystem->Shutdown(); }
    if (m_inputSystem) { m_inputSystem->Shutdown(); }
    if (p_window) { SDL_DestroyWindow(p_window); }
    SDL_Quit();
}

void WindowSystemImplSDL2::Update() {
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

bool WindowSystemImplSDL2::ShouldClose() {
    return m_shouldClose;
}

IRenderSystem* WindowSystemImplSDL2::RenderSystem() {
    return m_renderSystem.get();
}

IInputSystem* WindowSystemImplSDL2::InputSystem() {
    return m_inputSystem.get();
}