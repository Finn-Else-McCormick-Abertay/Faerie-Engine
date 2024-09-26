#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
//#include <allegro5/allegro_opengl.h>

#include <imgui.h>
#include <imgui_impl_allegro5.h>

int main() {
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_install_joystick();
    
    al_init_primitives_addon();

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
    al_set_window_title(display, "Untitled Tabletop Engine");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_joystick_event_source());

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
        }
        
        // Start the Dear ImGui frame
        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }

    // Cleanup
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_display(display);
    al_destroy_event_queue(queue);

    return 0;
}