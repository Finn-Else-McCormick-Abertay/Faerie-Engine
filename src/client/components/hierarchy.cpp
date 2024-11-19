#include "hierarchy.h"

#include <systems/logger.h>
#include <systems/ecs.h>

#include <util/locate_component.h>

Components::Hierarchy::Hierarchy() : m_self(FindOwningEntity(*this)) {}

Entity Components::Hierarchy::Parent() const { return m_parent; }

void Components::Hierarchy::ClearParent() {
    SetParent(Entity());
}
void Components::Hierarchy::SetParent(Entity newParent) {
    if (m_parent == newParent) { return; }

    __Internal_SetParent(newParent);
    if (m_parent) {
        if (m_self) { m_parent.Get<Hierarchy>().__Internal_AddChild(m_self); }
    }
}

void Components::Hierarchy::AddChild(Entity ent, bool force) {
    if (!ent) { return; }
    
    auto& hierarchy = ent.Get<Hierarchy>();
    if (hierarchy.Parent()) {
        if (!force) { return; }

        hierarchy.Parent().Get<Hierarchy>().RemoveChild(ent);
    }
    __Internal_AddChild(ent);
    if (m_self) { ent.Get<Hierarchy>().__Internal_SetParent(m_self); }
}

void Components::Hierarchy::__Internal_SetParent(Entity newParent) {
    if (m_parent == newParent) { return; }

    if (m_self) {
        if (m_parent) {
            Logger::Warning(*this, "Reparenting non-orphan entity.");
            m_parent.Get<Hierarchy>().RemoveChild(m_self);
        }
        m_parent = newParent;
    }
}

void Components::Hierarchy::__Internal_AddChild(Entity ent) {
    if (!ent) { return; }
    if (!m_childRoot) {
        m_childRoot = ent;
        auto& hierarchy = ent.Get<Hierarchy>();
        hierarchy.m_nextSibling = Entity();
        hierarchy.m_prevSibling = Entity();
        return;
    }

    Entity current = m_childRoot; Entity prev;
    while (current) {
        prev = current;
        current = current.Get<Hierarchy>().m_nextSibling;
    }
    prev.Get<Hierarchy>().m_nextSibling = ent;
    auto& hierarchy = ent.Get<Hierarchy>();
    hierarchy.m_nextSibling = Entity();
    hierarchy.m_prevSibling = prev;
}

void Components::Hierarchy::RemoveChild(Entity ent) {
    if (!ent || !m_childRoot) { return; }

    bool found = false;

    if (m_childRoot == ent) {
        found = true;
        auto& rootHierarchy = m_childRoot.Get<Hierarchy>();
        if (rootHierarchy.m_nextSibling) {
            m_childRoot = rootHierarchy.m_nextSibling;
            m_childRoot.Get<Hierarchy>().m_prevSibling = Entity();
            rootHierarchy.m_nextSibling = Entity();
        }
        else {
            m_childRoot = Entity();
        }
    }
    else {
        Entity current = m_childRoot;
        while (current) {
            if (current == ent) {
                found = true;
                auto& currentHierarchy = current.Get<Hierarchy>();
                Entity prev = currentHierarchy.m_prevSibling;
                Entity next = currentHierarchy.m_nextSibling;

                if (prev) { prev.Get<Hierarchy>().m_nextSibling = next; }
                if (next) { next.Get<Hierarchy>().m_prevSibling = prev; }

                currentHierarchy.m_nextSibling = Entity();
                currentHierarchy.m_prevSibling = Entity();
                break;
            }
            current = current.Get<Hierarchy>().m_nextSibling;
        }
    }

    if (found) { ent.Get<Hierarchy>().m_parent = Entity(); }
}

void Components::Hierarchy::ForEachChild(std::function<void(Entity)> callback) {
    Entity current = m_childRoot;
    while (current) {
        callback(current);
        current = current.Get<Hierarchy>().m_nextSibling;
    }
}

bool Components::Hierarchy::HasChild(Entity ent) const {
    Entity current = m_childRoot;
    while (current) {
        if (current == ent) { return true; }
        current = current.Get<Hierarchy>().m_nextSibling;
    }
    return false;
}

bool Components::Hierarchy::HasSibling(Entity ent) const {
    Entity current = m_nextSibling;
    while (current) {
        if (current == ent) { return true; }
        current = current.Get<Hierarchy>().m_nextSibling;
    }
    current = m_prevSibling;
    while (current) {
        if (current == ent) { return true; }
        current = current.Get<Hierarchy>().m_prevSibling;
    }
    return false;
}