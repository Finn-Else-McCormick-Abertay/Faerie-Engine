#include <systems/window_system.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/ecs.h>
#include <systems/logger.h>

#include <components/model.h>
#include <components/transform.h>

int main(int argc, char *argv[]) {

    if (!Logger::Instance().Init()) { return -1; }

    WindowSystem windowSystem;
    if (!windowSystem.Init()) { return -1; }
    
    if (!ResourceManager::Instance().Init()) { return -1; }
    
	if (!ECS::Instance().Init()) { return -1; }

    if (!ScriptingSystem::Instance().Init()) { return -1; }

	auto& registry = ECS::Registry();
	auto ent = registry.create();
	registry.emplace<Components::Transform>(ent);
	auto& model = registry.emplace<Components::Model>(ent);
    model.shaderId = ResourceManager::Load(ResourceInfo<Shader>("/resources/shaders/vert.glsl", "/resources/shaders/frag.glsl"));

    auto scriptId = ResourceManager::Load<Script>("/resources/rust_module_example.wasm");
    auto& script = ResourceManager::Get<Script>(scriptId);
    script.Call("run", {});

    // Main loop
    while (!windowSystem.ShouldClose()) {
        windowSystem.Update();
    }

    windowSystem.Shutdown();
    ECS::Instance().Shutdown();
    ScriptingSystem::Instance().Shutdown();
    ResourceManager::Instance().Shutdown();
    Logger::Instance().Shutdown();

    return 0;
}