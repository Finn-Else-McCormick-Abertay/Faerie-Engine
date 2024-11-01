#pragma once

#include <systems/system.h>

#include <entt/entt.hpp>

class ECS final : public ISystem
{
public:
    SYSTEM_LOGGER_NAME(ECS)

    static ECS& Instance();

    static entt::registry& Registry();

private:
    ECS() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    entt::registry m_registry;
};