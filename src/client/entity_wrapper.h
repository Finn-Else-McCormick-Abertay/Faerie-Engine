#pragma once

#include <systems/ecs.h>
#include <systems/logger.h>
#include <typeinfo>
#include <iostream>

class Entity
{
public:
    Entity(entt::entity ent = entt::null) : m_entity(ent) {}

    template<typename Component> bool Has() const {
        return ECS::Registry().try_get<Component>(m_entity) != nullptr;
    }

    template<typename Component> Component& Get() const {
        auto component = ECS::Registry().try_get<Component>(m_entity);
        if (!component) {
            Logger::Error("Entity",
                "Failed to get component ", typeid(Component).name(), " on entity ", *this, "; entity has no component of this type."
            );
        }
        return *component;
    }

    template<typename Component, typename... Args> Component& Add(Args&&... args) {
        if (Has<Component>()) {
            Logger::Warning("Entity",
                "Adding component ", typeid(Component).name(), " to entity ", *this, ", which already has an instance of this component. Will be replaced."
            );
        }
        return ECS::Registry().emplace_or_replace<Component>(m_entity, std::forward<Args>(args)...);
    }

    template<typename Component> Component& Erase() {
        if (Has<Component>()) {
            return ECS::Registry().erase<Component>(m_entity);
        }
        else {
            Logger::Warning("Entity",
                "Attempted to erase component ", typeid(Component).name(), " from entity ", *this, ", which has no such component."
            );
        }
    }

    inline operator bool() const { return m_entity == entt::null; }

    inline bool operator ==(Entity rhs) const { return m_entity == rhs.m_entity; }
    inline bool operator !=(Entity rhs) const { return !(*this == rhs); }

    inline entt::entity Raw() const { return m_entity; }

private:
    entt::entity m_entity;

    friend std::ostream& operator <<(std::ostream& os, const Entity& v);
};

inline std::ostream& operator <<(std::ostream& os, const Entity& v) { os << (unsigned int)v.m_entity; return os; }