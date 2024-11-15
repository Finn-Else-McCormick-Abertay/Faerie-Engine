#include "render_system.h"

#include <imgui.h>

#include <systems/ecs.h>
#include <components/transform.h>

#include <string>

void IRenderSystem::ImGuiRender() {
    BeginImGuiFrame();

    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

    auto view = ECS::Registry().view<Components::Transform>();
    for (auto entity : view) {
        auto [trans] = view.get(entity);
        ImGui::Begin(std::to_string((int)entity).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
        vec3 pos = trans.Position(); float posTemp[3] = { pos.x, pos.y, pos.z };
        if (ImGui::DragFloat3("Position", posTemp)) { trans.SetPosition(vec3(posTemp[0], posTemp[1], posTemp[2])); }
        ImGui::End();
    } 

    // Rendering
    ImGui::Render();
}