#include <systems/window_system.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/ecs.h>
#include <systems/logger.h>

#include <components/mesh.h>
#include <components/transform.h>
#include <components/hierarchy.h>
#include <components/camera.h>
#include <components/material.h>

#include <resources/model.h>
#include <resources/material.h>

int main(int argc, char *argv[]) {

    if (!Logger::Instance().Init()) { return -1; }
    if (!WindowSystem::Instance().Init()) { return -1; }
    if (!ResourceManager::Instance().Init()) { return -1; }
	if (!ECS::Instance().Init()) { return -1; }
    if (!ScriptingSystem::Instance().Init()) { return -1; }

    auto ent1 = ECS::Create(); auto& hier1 = ent1.Add<Components::Hierarchy>();

    auto ent2 = ECS::Create(); auto& hier2 = ent2.Add<Components::Hierarchy>();

    //auto textureId = ResourceManager::Load<Texture>("/resources/textures/test.png");

    auto modelId = ResourceManager::Load<Model>("/resources/models/suzanne.fbx");
    auto& model = ResourceManager::Get<Model>(modelId);
    
    auto shaderIdGreen = ResourceManager::Load(ResourceInfo<Shader>("/resources/shaders/vert.glsl", "/resources/shaders/frag.glsl"));
    auto materialIdGreen = ResourceManager::Emplace(ResourceInfo<Material>("MATERIAL_GREEN"), Material(shaderIdGreen));

    auto shaderIdRed = ResourceManager::Load(ResourceInfo<Shader>("/resources/shaders/vert.glsl", "/resources/shaders/frag_red.glsl"));
    auto materialIdRed = ResourceManager::Emplace(ResourceInfo<Material>("MATERIAL_RED"), Material(shaderIdRed));

    model.Instantiate(ent1);
    hier1.ForEachDescendant([&](Entity descendant){
        descendant.AddWithArgs<Components::Material>(materialIdGreen);
    });
    ent1.Get<Components::Transform>().Move(vec3(-130.f, 0.f, 150.f));

    model.Instantiate(ent2);
    hier2.ForEachDescendant([&](Entity descendant){
        descendant.AddWithArgs<Components::Material>(materialIdRed);
    });
    ent2.Get<Components::Transform>().Move(vec3(130.f, 0.f, 150.f));

    /*
    auto scriptId = ResourceManager::Load<Script>("/resources/rust_module_example.wasm");
    auto& script = ResourceManager::Get<Script>(scriptId);
    script.Call("run", {});
    */

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