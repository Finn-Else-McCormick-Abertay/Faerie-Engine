#include "ecs.h"

#include <systems/logger.h>

ECS& ECS::Instance() {
    static ECS instance;
    return instance;
}

bool ECS::InitImpl() {
    Logger::Info(*this, "Initialised");
    return true;
}

void ECS::ShutdownImpl() {
    
}

entt::registry& ECS::Registry() {
    return Instance().m_registry;
}