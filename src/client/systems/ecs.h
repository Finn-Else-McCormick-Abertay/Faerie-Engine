#pragma once

#include <systems/system.h>

#include <entt/entt.hpp>

class ECS : public ISystem
{
public:
    static ECS& Instance();

    static entt::registry& Registry();

private:
    ECS() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    entt::registry m_registry;
};