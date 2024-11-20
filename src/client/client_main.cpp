#include <systems/window_system.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/ecs.h>
#include <systems/logger.h>

#include <components/model.h>
#include <components/transform.h>
#include <components/hierarchy.h>
#include <components/camera.h>

int main(int argc, char *argv[]) {

    if (!Logger::Instance().Init()) { return -1; }

    if (!WindowSystem::Instance().Init()) { return -1; }
    
    if (!ResourceManager::Instance().Init()) { return -1; }
    
	if (!ECS::Instance().Init()) { return -1; }

    if (!ScriptingSystem::Instance().Init()) { return -1; }

    auto parentEnt = ECS::Create();
    parentEnt.Add<Components::Hierarchy>();

	auto entity = ECS::Create();
    auto [trans, hier] = entity.Add<Components::Transform, Components::Hierarchy>();
    hier.SetParent(parentEnt);
    trans.Move(glm::vec3(2.f, 0.f, 0.f));
	auto& model = entity.Add<Components::Model>();
    model.shaderId = ResourceManager::Load(ResourceInfo<Shader>("/resources/shaders/vert.glsl", "/resources/shaders/frag.glsl"));

    auto scriptId = ResourceManager::Load<Script>("/resources/rust_module_example.wasm");
    auto& script = ResourceManager::Get<Script>(scriptId);
    script.Call("run", {});

    auto cameraEntity = ECS::Create();
    cameraEntity.Add<Components::PerspectiveCamera, Components::Transform>();

    WindowSystem::Instance().RenderSystem()->SetActiveCamera(cameraEntity);

    WindowSystem::MainLoop();

    // Cleanup
    WindowSystem::Instance().Shutdown();
    ECS::Instance().Shutdown();
    ScriptingSystem::Instance().Shutdown();
    ResourceManager::Instance().Shutdown();
    Logger::Instance().Shutdown();

    return 0;
}