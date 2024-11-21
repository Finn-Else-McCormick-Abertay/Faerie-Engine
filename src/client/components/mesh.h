#pragma once

#include <resources/resource_info.h>

namespace Components
{
    class Mesh
    {
    public:
        Mesh(ResourceIdentifier meshId = 0);

        ResourceIdentifier MeshId() const;

    private:
        ResourceIdentifier m_meshId;
    };

} // Components