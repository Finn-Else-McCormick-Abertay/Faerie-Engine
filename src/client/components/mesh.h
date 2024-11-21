#pragma once

#include <resources/resource_info.h>

namespace Components
{
    struct Mesh
    {
        ResourceIdentifier shaderId;
        ResourceIdentifier meshId;
    
        Mesh(ResourceIdentifier meshId);
        Mesh();
    };

} // Components