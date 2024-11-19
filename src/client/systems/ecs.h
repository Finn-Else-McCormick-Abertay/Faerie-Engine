#pragma once

#include <systems/system.h>

#include <entt/entt.hpp>

class Entity;

class ECS final : public ISystem
{
public:
    static ECS& Instance();

    static entt::registry& Registry();

    static Entity Create();

private:
    ECS() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    entt::registry m_registry;
};