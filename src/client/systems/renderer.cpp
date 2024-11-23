#include "renderer.h"
#include <systems/system_lifecycle_define.h>

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

#ifdef OPENGL3
#include <systems/platform/renderer_opengl3.h>
FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_POLYMORPHIC(Renderer, RendererOpenGl3)
#endif
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(Renderer)

void Renderer::ImGuiRender() {
    BeginImGuiFrame();

    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

    auto& debug = dynamic_cast<IDebugSystemInternal&>(Debug::Instance());
    debug.__Internal_RunImGuiCallbacks();
    
    // Rendering
    ImGui::Render();
}

Entity Renderer::ActiveCamera() const { return m_cameraEntity; }
void Renderer::SetActiveCamera(Entity ent) {
    if (!ent.Has<Components::PerspectiveCamera>() && !ent.Has<Components::OrthoCamera>()) {
        Logger::Warning(*this, ent, " set as active camera, but has no camera component.");
    }
    m_cameraEntity = ent;
}