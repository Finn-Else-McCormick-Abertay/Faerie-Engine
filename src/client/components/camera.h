#pragma once

#include <maths_types.h>
#include <memory>
#include <entity_wrapper.h>

namespace Components
{
    class PerspectiveCamera {
    public:
        PerspectiveCamera(float fov = glm::radians(90.f), float near = 0.1f, float far = 1000.f);
        
        mat4 ProjectionMatrix() const;

        float FieldOfView() const;
        float NearPlane() const; float FarPlane() const;

        void SetFieldOfView(float);
        void SetNearPlane(float); void SetFarPlane(float);
        
        void SetAspect(float);
        void __OnAdded(Entity);
    private:
        float m_fov, m_near, m_far, m_aspect;
        mat4 m_projectionMatrix;

        void UpdateMatrix();
    };

    class OrthoCamera {
    public:
        OrthoCamera();

        mat4 ProjectionMatrix() const;
        
        void __OnAdded(Entity);
    private:
        float m_left, m_right, m_bottom, m_top;
        mat4 m_projectionMatrix;
        
        void UpdateMatrix();
    };
    
} // Components