#include "window_system.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>

#ifdef OPENGL3
#include <systems/platform/render_system_opengl3.h>
#endif // OPENGL3

#include <systems/logger.h>
#include <systems/ecs.h>
#include <systems/input.h>

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
    ImGuiIO& imIo = ImGui::GetIO();
    auto& input = dynamic_cast<IInputSystemInternal&>(Input::Instance());

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

        // Device changes
        switch (event.type) {
            // Audio
            case SDL_AUDIODEVICEADDED: {
                // UNUSED
            } break;
            case SDL_AUDIODEVICEREMOVED: {
                // UNUSED
            } break;

            // Controller
            case SDL_CONTROLLERDEVICEADDED: {
                // UNUSED
            } break;
            case SDL_CONTROLLERDEVICEREMOVED: {
                // UNUSED
            } break;
            case SDL_CONTROLLERDEVICEREMAPPED: {
                // UNUSED
            } break;
            case SDL_CONTROLLERSTEAMHANDLEUPDATED: {
                // UNUSED
            } break;
        }

        if (windowFlags & SDL_WINDOW_INPUT_FOCUS) {
            if (!imIo.WantCaptureKeyboard) {
                // Keyboard events
                switch (event.type) {
                    case SDL_KEYDOWN:
                    case SDL_KEYUP: {
                        input.SendKeyEvent(event.key.keysym.scancode, event.key.state, event.key.repeat > 0, event.key.keysym.mod);
                    } break;
                    case SDL_TEXTINPUT: {
                        // UNUSED
                    } break;
                }
            }

            // Controller events
            // (Note - This doesn't currently support non-controller joysticks but could with just a little modification)
            switch (event.type) {
                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP: {
                    input.SendControllerButtonEvent(
                        static_cast<SDL_GameControllerButton>(event.cbutton.button),
                        event.cbutton.state, event.cbutton.which
                    );
                } break;
                case SDL_CONTROLLERAXISMOTION: {
                    // Convert to float in range -1 to 1
                    float value = static_cast<float>(event.caxis.value) / SDL_MAX_SINT16;
                    input.SendControllerAxisEvent(
                        static_cast<SDL_GameControllerAxis>(event.caxis.axis),
                        value, event.caxis.which
                    );
                } break;
                case SDL_CONTROLLERTOUCHPADDOWN:
                case SDL_CONTROLLERTOUCHPADUP: {
                    // UNUSED
                } break;
                case SDL_CONTROLLERTOUCHPADMOTION: {
                    // UNUSED
                } break;
            }

            // File drag and drop events
            switch (event.type) {
                case SDL_DROPFILE: {
                    // UNUSED
                } break;
                case SDL_DROPTEXT: {
                    // UNUSED
                } break;
                case SDL_DROPBEGIN: {
                    // UNUSED
                } break;
                case SDL_DROPCOMPLETE: {
                    // UNUSED
                } break;
            }
        }

        if ((windowFlags & SDL_WINDOW_MOUSE_FOCUS) && !imIo.WantCaptureMouse) {
            // Mouse events
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: {
                    input.SendMouseButtonEvent(
                        event.button.button, event.button.state, event.button.clicks > 1,
                        int2(event.button.x, event.button.y), event.button.which
                    );
                } break;
                case SDL_MOUSEMOTION: {
                    input.SendMouseMotionEvent(
                        int2(event.motion.x, event.motion.y), int2(event.motion.xrel, event.motion.yrel), event.motion.which
                    );
                } break;
                case SDL_MOUSEWHEEL: {
                    vec2 motion = vec2(event.wheel.preciseX, event.wheel.preciseY);
                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) { motion *= -1; }
                    input.SendMouseWheelEvent(motion, event.wheel.which);
                } break;
            }

            // Touch events
            switch (event.type) {
                case SDL_FINGERDOWN:
                case SDL_FINGERUP: {
                    // UNUSED
                } break;
                case SDL_FINGERMOTION: {
                    // UNUSED
                } break;
                
                case SDL_DOLLARGESTURE: {
                    // UNUSED
                } break;
                case SDL_DOLLARRECORD: {
                    // UNUSED
                } break;
                case SDL_MULTIGESTURE: {
                    // UNUSED
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

IRenderSystem& WindowSystem::RenderSystem() {
    return *m_renderSystem.get();
}

int2 WindowSystem::WindowSize() {
    return Instance().m_windowSize;
}

float WindowSystem::WindowAspect() {
    auto& inst = Instance();
    return static_cast<float>(inst.m_windowSize.x) / static_cast<float>(inst.m_windowSize.y);
}