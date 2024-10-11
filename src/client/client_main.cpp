#include <SDL.h>
//#include <SDL_syswm.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <systems/window_system.h>
#include <systems/ecs.h>
#include <systems/scripting_system.h>
#include <systems/impl/scripting_system_impl_lua.h>

#include <components/model.h>
#include <components/transform.h>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

int main(int argc, char *argv[]) {
	if (!ECS::Instance().Init()) { return -1; }

    WindowSystem windowSystem;
    if (!windowSystem.Init()) { return -1; }
	
	ScriptingSystemImplLua scriptingSystem;
	if (!scriptingSystem.Init()) { return -1; }

	auto& registry = ECS::Registry();
	auto ent = registry.create();
	registry.emplace<Components::Transform>(ent);
	registry.emplace<Components::Model>(ent);

    // Main loop
    while (!windowSystem.ShouldClose()) {
        windowSystem.Update();
    }

    return 0;
}