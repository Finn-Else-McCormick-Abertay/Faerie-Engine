#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <components/hierarchy.h>
#include <util/locate_component.h>

Components::Transform::Transform() : m_position(0,0,0), m_scale(1,1,1), m_rotation(1,0,0,0), m_self(FindOwningEntity(*this)) {
    UpdateMatrices();
}

vec3 Components::Transform::Position() const { return m_position; }
vec3 Components::Transform::Scale() const { return m_scale; }
quat Components::Transform::Rotation() const { return m_rotation; }

vec3 Components::Transform::GlobalPosition() const {
    if (auto hier = m_self.TryGet<Hierarchy>(); hier) {
        if (auto parentTrans = hier->Parent().TryGet<Transform>(); parentTrans) {
            vec4 globalPos = parentTrans->GlobalMatrix() * glm::vec4(Position(), 1.f);
            return vec3(globalPos);
        }
    }
    return Position();
}

mat4 Components::Transform::LocalMatrix() const { return m_localTransform; }
mat4 Components::Transform::GlobalMatrix() const { return m_globalTransform; }


void Components::Transform::SetPosition(const vec3& v) {
    m_position = v; UpdateMatrices();
}

void Components::Transform::SetScale(const vec3& v) {
    m_scale = v; UpdateMatrices();
}

void Components::Transform::SetRotation(const quat& v) {
    m_rotation = v; UpdateMatrices();
}

void Components::Transform::Move(const vec3& v) {
    SetPosition(Position() + v);
}

void Components::Transform::UpdateMatrices() {
    m_localTransform = glm::translate(m_position) * glm::mat4_cast(m_rotation) * glm::scale(m_scale);
    if (auto hier = m_self.TryGet<Hierarchy>(); hier) {
        if (auto parentTrans = hier->Parent().TryGet<Transform>(); parentTrans) {
            m_globalTransform = parentTrans->m_globalTransform * m_localTransform;
        }
        else { m_globalTransform = m_localTransform; }
        hier->ForEachChild([](Entity child){
            if (auto childTrans = child.TryGet<Transform>(); childTrans) {
                childTrans->UpdateMatrices();
            }
        });
    }
    else { m_globalTransform = m_localTransform; }
}