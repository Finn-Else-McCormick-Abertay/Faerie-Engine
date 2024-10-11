#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Components
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;

        Transform();
        glm::mat4 AsMatrix() const;
    };

} // Components