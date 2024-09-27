#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "GL/glu.h"

#include <imgui.h>
#include <imgui_impl_allegro5.h>

int main() {
    // Initialise Allegro
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_install_joystick();
    al_install_touch_input();
    
    al_init_image_addon();
    al_init_primitives_addon(); // Required for ImGui
    
    // Create display (window)
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    al_set_window_title(display, "Untitled Tabletop Engine");
    //al_set_display_icon(display, /* Icon Bitmap */);

    // Setup event queue
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_joystick_event_source());
    al_register_event_source(queue, al_get_touch_input_event_source());

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplAllegro5_Init(display);

    io.Fonts->AddFontDefault();


    /*
    ALLEGRO_SHADER* shader = al_create_shader(ALLEGRO_SHADER_GLSL);
    al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, "src/shaders/vert.glsl");
    al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, "src/shaders/frag.glsl");
    al_build_shader(shader);
    al_use_shader(shader);
    */


    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(queue, &event)) {
            ImGui_ImplAllegro5_ProcessEvent(&event);
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
            }

            // --------------------
            // -- Keyboard Input --
            // --------------------

            if (!io.WantCaptureKeyboard) {
                if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

                }
                if (event.type == ALLEGRO_EVENT_KEY_UP) {

                }
                // For text input. Supports modifiers, auto-repeat etc
                if (event.type == ALLEGRO_EVENT_KEY_CHAR) {

                }
            }

            // -----------------
            // -- Mouse Input --
            // -----------------

            if (!io.WantCaptureMouse) {
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                }
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    
                }
                if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                    
                }
            }

            // ----------------------
            // -- Controller Input --
            // ----------------------

            // Fired when a controller connects or disconnects
            if (event.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION) {
                al_reconfigure_joysticks();
            }
            if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {

            }
            if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
                
            }
            if (event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {

            }

            // -----------------------
            // -- Touchscreen Input --
            // -----------------------

            if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN) {

            }
            if (event.type == ALLEGRO_EVENT_TOUCH_MOVE) {

            }
            if (event.type == ALLEGRO_EVENT_TOUCH_END) {
                
            }
            if (event.type == ALLEGRO_EVENT_TOUCH_CANCEL) {
                
            }
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingOverCentralNode);

        ImGui::Render();

        // Render
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        glVertex2f(-100, -100);
        glVertex2f(+100, -100);
        glVertex2f(+100, +100);
        glVertex2f(-100, +100);
        glEnd();

        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        
        al_flip_display();
        al_wait_for_vsync();
    }

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}