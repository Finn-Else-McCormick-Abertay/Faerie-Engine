#include "transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

Components::Transform::Transform() : position(0,0,0), scale(1,1,1), rotation(1,0,0,0) {}

glm::mat4 Components::Transform::AsMatrix() const {
    return glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);
}