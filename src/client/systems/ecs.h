#pragma once

#include <systems/system_lifecycle_declare.h>

#include <entt/entt.hpp>

class Entity;

class ECS final {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ECS)
public:
    static entt::registry& Registry();

    static Entity Create();

private:
    entt::registry m_registry;
};