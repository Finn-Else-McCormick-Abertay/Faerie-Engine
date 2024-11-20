#include "camera.h"

#include <systems/window_system.h>
#include <components/transform.h>

using namespace Components;

PerspectiveCamera::PerspectiveCamera(float fov, float near, float far) : m_fov(fov), m_near(near), m_far(far), m_aspect(WindowSystem::WindowAspect()) {
    UpdateMatrix();
}

void PerspectiveCamera::__OnAdded(Entity ent) {
    ent.GetOrAdd<Transform>();
}

float PerspectiveCamera::FieldOfView() const { return m_fov; }
float PerspectiveCamera::NearPlane() const { return m_near; }
float PerspectiveCamera::FarPlane() const { return m_far; }

void PerspectiveCamera::SetFieldOfView(float v) { m_fov = v; UpdateMatrix(); }
void PerspectiveCamera::SetNearPlane(float v) { m_near = v; UpdateMatrix(); }
void PerspectiveCamera::SetFarPlane(float v) { m_far = v; UpdateMatrix(); }

void PerspectiveCamera::SetAspect(float v) { m_aspect = v; UpdateMatrix(); }

void PerspectiveCamera::UpdateMatrix() {
    m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

mat4 PerspectiveCamera::ProjectionMatrix() const { return m_projectionMatrix; }


OrthoCamera::OrthoCamera() : m_left(0.f), m_right(0.f), m_bottom(0.f), m_top(0.f) {
    UpdateMatrix();
}

void OrthoCamera::__OnAdded(Entity ent) {
    ent.GetOrAdd<Transform>();
}

void OrthoCamera::UpdateMatrix() {
    m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top);
}

mat4 OrthoCamera::ProjectionMatrix() const { return m_projectionMatrix; }