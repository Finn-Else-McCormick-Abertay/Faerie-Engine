#pragma once

#include <maths_types.h>

namespace Components
{
    class Transform
    {
    public:
        Transform();
        
        mat4 LocalMatrix() const;
        mat4 GlobalMatrix() const;

        vec3 Position() const;
        vec3 Scale() const;
        quat Rotation() const;

        void SetPosition(const vec3&);
        void SetScale(const vec3&);
        void SetRotation(const quat&);

        void Move(const vec3&);
    private:
        vec3 m_position;
        vec3 m_scale;
        quat m_rotation;
        mat4 m_localTransform, m_globalTransform;

        void UpdateMatrices();
    };

} // Components