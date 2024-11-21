#include "window_system.h"

#ifdef OPENGL3
#include <systems/platform/render_system_opengl3.h>
#endif // OPENGL3

#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>

#include <systems/logger.h>
#include <systems/ecs.h>
#include <components/camera.h>

WindowSystem& WindowSystem::Instance() {
    static WindowSystem windowSystem;
    return windowSystem;
}

bool WindowSystem::InitImpl() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        Logger::Error(*this, "SDL_Init: ", SDL_GetError());
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
        Logger::Error(*this, "Failed to initialise RenderSystem");
        return false;
    }

    p_window = SDL_CreateWindow("Faerie Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
    if (p_window == nullptr) {
        Logger::Error(*this, "SDL_CreateWindow: ", SDL_GetError());
        return false;
    }

    m_renderSystem->CreateContext(p_window);

    int width, height;
    SDL_GetWindowSize(p_window, &width, &height);
    m_windowSize = int2(width, height);

    Logger::Info(*this, "Initialised");
    return true;
}

void WindowSystem::ShutdownImpl() {
    if (m_renderSystem) { m_renderSystem->Shutdown(); }
    if (p_window) { SDL_DestroyWindow(p_window); }
    SDL_Quit();
}

void WindowSystem::MainLoop() {
    auto& inst = Instance();
    while (!inst.m_shouldClose) { inst.Update(); }
}

void WindowSystem::Update() {
    auto windowFlags = SDL_GetWindowFlags(p_window);
    ImGuiIO& io = ImGui::GetIO();
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) { m_shouldClose = true; }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(p_window)) {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE: {
                    m_shouldClose = true;
                } break;
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    m_windowSize = int2(event.window.data1, event.window.data2);
                    auto view = ECS::Registry().view<Components::PerspectiveCamera>();
                    for (auto& entity : view) {
                        auto& camera = view.get<Components::PerspectiveCamera>(entity);
                        camera.SetAspect(WindowAspect());
                    }
                } break;
            }
        }

        if ((windowFlags & SDL_WINDOW_INPUT_FOCUS) && !io.WantCaptureKeyboard) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    
                } break;
                case SDL_KEYUP: {
                    
                } break;
                case SDL_TEXTINPUT: {

                } break;
            }
        }

        if ((windowFlags & SDL_WINDOW_MOUSE_FOCUS) && !io.WantCaptureMouse) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN: {
                    
                } break;
                case SDL_MOUSEBUTTONUP: {
                    
                } break;
                case SDL_MOUSEMOTION: {
                    
                } break;
                case SDL_MOUSEWHEEL: {
                    
                } break;
            }
        }
    }
    if (windowFlags & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(10);
        return;
    }

    m_renderSystem->ImGuiRender();
    m_renderSystem->Render();
}

IRenderSystem* WindowSystem::RenderSystem() {
    return m_renderSystem.get();
}

int2 WindowSystem::WindowSize() {
    return Instance().m_windowSize;
}

float WindowSystem::WindowAspect() {
    auto& inst = Instance();
    return static_cast<float>(inst.m_windowSize.x) / static_cast<float>(inst.m_windowSize.y);
}