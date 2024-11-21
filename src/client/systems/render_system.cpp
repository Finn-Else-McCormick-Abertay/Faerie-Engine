#include "render_system.h"

#include <imgui.h>

#include <systems/logger.h>
#include <systems/ecs.h>
#include <components/transform.h>
#include <components/mesh.h>
#include <components/camera.h>

#include <string>
#include <sstream>
#include <entity_wrapper.h>

void IRenderSystem::ImGuiRender() {
    BeginImGuiFrame();

    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

    auto view = ECS::Registry().view<Components::Transform>();
    for (auto entity : view) {
        auto [trans] = view.get(entity);
        std::stringstream ss; ss << Entity(entity);
        ImGui::Begin(ss.str().c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
        vec3 pos = trans.Position(); float posTemp[3] = { pos.x, pos.y, pos.z };
        if (ImGui::DragFloat3("Position", posTemp)) { trans.SetPosition(vec3(posTemp[0], posTemp[1], posTemp[2])); }
        ImGui::End();
    }
    
    // Rendering
    ImGui::Render();
}

Entity IRenderSystem::ActiveCamera() const { return m_cameraEntity; }
void IRenderSystem::SetActiveCamera(Entity ent) {
    if (!ent.Has<Components::PerspectiveCamera>() && !ent.Has<Components::OrthoCamera>()) {
        Logger::Warning(*this, ent, " set as active camera, but has no camera component.");
    }
    m_cameraEntity = ent;
}