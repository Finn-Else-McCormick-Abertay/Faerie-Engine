#pragma once

#include <maths_types.h>
#include <entity_wrapper.h>
#include <json/json.h>

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

        vec3 GlobalPosition() const;

        void SetPosition(const vec3&);
        void SetScale(const vec3&);
        void SetRotation(const quat&);

        void Move(const vec3&);

        void __Serialise(Json::Value&) const;
        void __Deserialise(const Json::Value&);
    private:
        vec3 m_position; vec3 m_scale; quat m_rotation;
        mat4 m_localTransform, m_globalTransform;

        Entity m_self;

        void UpdateMatrices();
    };

} // Components