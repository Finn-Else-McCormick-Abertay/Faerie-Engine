#include <systems/window_system.h>
#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/ecs.h>
#include <systems/logger.h>
#include <systems/input.h>
#include <systems/debug.h>

#include <components/mesh.h>
#include <components/transform.h>
#include <components/hierarchy.h>
#include <components/camera.h>
#include <components/material.h>

#include <resources/model.h>
#include <resources/material.h>

#include <imgui.h>

int main(int argc, char *argv[]) {

    if (
        !Logger::Init() ||
        !WindowSystem::Init() ||
        !ResourceManager::Init() ||
        !ECS::Init() ||
        !ScriptingSystem::Init() ||
        !Input::Init() ||
        !Debug::Init()
    ) { return -1; }

    // Scene setup (TEMP)
    {
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

        RenderSystem::Instance().SetActiveCamera(cameraEntity);

        Debug::SetPersistent("positions", [](){
            auto view = ECS::Registry().view<Components::Transform>();
            for (auto entity : view) {
                auto [trans] = view.get(entity);
                std::stringstream ss; ss << Entity(entity);
                ImGui::Begin(ss.str().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
                vec3 pos = trans.Position(); float posTemp[3] = { pos.x, pos.y, pos.z };
                if (ImGui::DragFloat3("Position", posTemp)) { trans.SetPosition(vec3(posTemp[0], posTemp[1], posTemp[2])); }
                ImGui::End();
            }
        });
    }

    // Enter main loop
    WindowSystem::MainLoop();

    // Cleanup
    Debug::Shutdown();
    Input::Shutdown();
    WindowSystem::Shutdown();
    ECS::Shutdown();
    ScriptingSystem::Shutdown();
    ResourceManager::Shutdown();
    Logger::Shutdown();

    return 0;
}