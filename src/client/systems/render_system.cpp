#include "render_system.h"

#include <imgui.h>

#include <systems/logger.h>
#include <systems/ecs.h>
#include <systems/debug.h>

#include <components/transform.h>
#include <components/mesh.h>
#include <components/camera.h>

#include <string>
#include <sstream>
#include <entity_wrapper.h>

void IRenderSystem::ImGuiRender() {
    BeginImGuiFrame();

    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

    auto& debug = dynamic_cast<IDebugSystemInternal&>(Debug::Instance());
    debug.__Internal_RunImGuiCallbacks();
    
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