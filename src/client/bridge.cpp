#include "bridge.h"

#include <systems/ecs.h>
#include <entity_wrapper.h>
#include <systems/logger.h>

unsigned int bridge::CreateEntity() {
    return (unsigned int)ECS::Create().Id();
}