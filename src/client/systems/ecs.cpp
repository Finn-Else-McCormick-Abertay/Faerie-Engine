#include "ecs.h"

ECS& ECS::Instance() {
    static ECS instance;
    return instance;
}

bool ECS::InitImpl() {
    return true;
}

void ECS::ShutdownImpl() {
    
}

entt::registry& ECS::Registry() {
    return Instance().m_registry;
}