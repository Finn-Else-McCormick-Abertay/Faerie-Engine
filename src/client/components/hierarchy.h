#pragma once

#include <entity_wrapper.h>
#include <functional>

namespace Components
{
    class Hierarchy
    {
    public:
        Hierarchy();

        Entity Parent() const;

        void ForEachChild(std::function<void(Entity)>);

        bool HasChild(Entity) const;
        bool HasSibling(Entity) const;

        void SetParent(Entity); void ClearParent();

        void AddChild(Entity, bool force = false);
        void RemoveChild(Entity);

    private:
        void __Internal_AddChild(Entity);
        void __Internal_SetParent(Entity);

        Entity m_self;
        Entity m_parent, m_childRoot;
        Entity m_prevSibling, m_nextSibling;
    };

} // Components