#include <systems/window.h>
#include <systems/resource_manager.h>
#include <systems/script_engine.h>
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

#include <util/json_archive.h>

int main(int argc, char *argv[]) {
	if (!Logger::Init() ||
		!Window::Init() ||
		!ResourceManager::Init() ||
		!ECS::Init() ||
		!ScriptEngine::Init() ||
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

		//ScriptEngine::SetFunc("alert", [](){ printf("Imported function called.\n");});

		auto scriptId = ResourceManager::Load<Script>("/resources/rust_module_example_component_model.wasm");
		auto& script = ResourceManager::Get<Script>(scriptId);

		//script.Call("run", {});
		//auto returns = script.Call("number", {});
		//Logger::Info(script, vector_to_string(returns));

		auto cameraEntity = ECS::Create();
		cameraEntity.Add<Components::PerspectiveCamera, Components::Transform>();

		Renderer::Instance().SetActiveCamera(cameraEntity);

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
	Window::MainLoop();

	// Cleanup
	Debug::Shutdown();
	Input::Shutdown();
	Window::Shutdown();
	ECS::Shutdown();
	ScriptEngine::Shutdown();
	ResourceManager::Shutdown();
	Logger::Shutdown();

	return 0;
}