#pragma once

#include <resources/resource_info.h>
#include <entity_wrapper.h>

namespace Components
{
    class Material
    {
    public:
        Material(ResourceIdentifier material = 0);

        ResourceIdentifier MaterialId() const;

        void __OnAdded(Entity);
        void __OnRemoved(Entity);
    private:
        ResourceIdentifier m_materialId;
    };

    class ShaderHandle {};
    
} // Components