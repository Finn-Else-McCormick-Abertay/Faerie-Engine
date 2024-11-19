#pragma once

#include <systems/ecs.h>
#include <entity_wrapper.h>

template<typename Component>
Entity FindOwningEntity(const Component& comp) {
    Entity foundEntity;
    auto view = ECS::Registry().view<Component>();
    for (auto& entity : view) {
        auto& component = view.template get<Component>(entity);
        if (&component == &comp) {
            foundEntity = entity;
            break;
        }
    }
    return foundEntity;
}