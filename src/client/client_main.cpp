#include <systems/window_system.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/ecs.h>

#include <components/model.h>
#include <components/transform.h>

#include <wasmtime.hh>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

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

/*
    std::ifstream watFile;
    watFile.open(ResourceManager::Instance().RootPath() + "resources/hello.wat");
    std::stringstream strStream;
    strStream << watFile.rdbuf();
    std::string watText = strStream.str();

    wasmtime::Engine engine;
    printf("Compiling module\n");
    auto module = wasmtime::Module::compile(engine, watText).unwrap();

    printf("Initialising wasmtime\n");
    wasmtime::Store store(engine);

    printf("Creating callback\n");
    wasmtime::Func hostFunc = wasmtime::Func::wrap(store, [](){ printf("Callback called from module!\n"); });
    
    printf("Instantiating module\n");
    auto instance = wasmtime::Instance::create(store, module, {hostFunc}).unwrap();
    
    printf("Extracting export\n");
    auto run = std::get<wasmtime::Func>(*instance.get(store, "run"));

    printf("Calling export\n");
    run.call(store, {}).unwrap();
*/

    // Main loop
    while (!windowSystem.ShouldClose()) {
        windowSystem.Update();
    }

    windowSystem.Shutdown();
    ResourceManager::Instance().Shutdown();
    ECS::Instance().Shutdown();
    ScriptingSystem::Instance().Shutdown();

    return 0;
}