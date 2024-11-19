#pragma once

#include <systems/ecs.h>
#include <systems/logger.h>
#include <typeinfo>
#include <iostream>

class Entity
{
public:
    Entity(entt::entity ent = entt::null) : m_entity(ent) {}
    
    inline entt::entity Id() const { return m_entity; }
    
    inline bool Exists() const { return m_entity != entt::null; }
    inline operator bool() const { return Exists(); }
    inline bool Valid() const { return ECS::Registry().valid(m_entity); }
    inline bool Orphan() const { return ECS::Registry().orphan(m_entity); }
    
    inline bool operator ==(Entity rhs) const { return m_entity == rhs.m_entity; }
    inline bool operator !=(Entity rhs) const { return !(*this == rhs); }

public:
    template<typename... Components> bool Has() const { return ECS::Registry().all_of<Components...>(m_entity); }

    template<typename... Components> decltype(auto) Get() const {
        if constexpr(sizeof...(Components) == 1) {
            auto component = ECS::Registry().try_get<Components...>(m_entity);
            if (!component) { Logger::Error("Entity", "Failed to get component ", typeid(Components).name()..., " on ", *this, "; entity has no component of this type."); }
            return *component;
        }
        else { return std::forward_as_tuple(Get<Components>()...); }
    }

    template<typename Component, typename... Args> Component& Add(Args&&... args) {
        return __Internal_Add<Component>(std::forward<Args>(args)...);
    }

    template<typename... Components> decltype(auto) Add() {
        if constexpr(sizeof...(Components) == 1) {
            return __Internal_Add<Components...>();
        }
        else { return std::forward_as_tuple(Add<Components>()...); }
    }

    template<typename... Components> void Erase() {
        if constexpr(sizeof...(Components) == 1) {
            if (Has<Components...>()) {
                ECS::Registry().erase<Components...>(m_entity);
            }
            else { Logger::Warning("Entity", "Attempted to erase component ", typeid(Components).name()..., " from ", *this, ", which has no such component."); }
        }
        else { (Erase<Components>(), ...); }
    }

private:
    template<typename Component, typename... Args> Component& __Internal_Add(Args&&... args) {
        if (Has<Component>()) { Logger::Warning("Entity", "Adding component ", typeid(Component).name(), " to ", *this, ", which already has an instance of this component. Will be replaced."); }
        return ECS::Registry().emplace_or_replace<Component>(m_entity, std::forward<Args>(args)...);
    }

private:
    entt::entity m_entity;
};

inline std::ostream& operator <<(std::ostream& os, const Entity& entity) {
    os << "Entity<";
    if (entity.Exists()) { os << (unsigned int)entity.Id(); } else { os << "NULL"; }
    os << ">";
    return os;
}