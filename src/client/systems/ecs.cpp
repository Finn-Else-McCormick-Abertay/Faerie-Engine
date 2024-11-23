#include "ecs.h"
#include <systems/system_lifecycle_define.h>

#include <entity_wrapper.h>
#include <systems/logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ECS)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ECS)

bool ECS::__Internal_Init() {
    return true;
}

void ECS::__Internal_Shutdown() {}

entt::registry& ECS::Registry() {
    return Instance().m_registry;
}

Entity ECS::Create() {
    return Entity(Registry().create());
}