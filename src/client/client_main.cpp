#include <SDL.h>
//#include <SDL_syswm.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <systems/window_system.h>
#include <systems/ecs.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/impl/scripting_system_impl_lua.h>

#include <components/model.h>
#include <components/transform.h>

int main(int argc, char *argv[]) {

    WindowSystem windowSystem;
    if (!windowSystem.Init()) { return -1; }
    
    if (!ResourceManager::Instance().Init()) { return -1; }
    
	if (!ECS::Instance().Init()) { return -1; }
	
	ScriptingSystemImplLua scriptingSystem;
	if (!scriptingSystem.Init()) { return -1; }

	auto& registry = ECS::Registry();
	auto ent = registry.create();
	registry.emplace<Components::Transform>(ent);
	auto& model = registry.emplace<Components::Model>(ent);
    model.vertexShader = "resources/shaders/vert.glsl";
    model.fragmentShader = "resources/shaders/frag.glsl";

    ResourceManager::Load<Shader, const std::string&, const std::string&>(model.vertexShader, model.fragmentShader);

    // Main loop
    while (!windowSystem.ShouldClose()) {
        windowSystem.Update();
    }

    windowSystem.Shutdown();
    ResourceManager::Instance().Shutdown();
    ECS::Instance().Shutdown();
    scriptingSystem.Shutdown();

    return 0;
}