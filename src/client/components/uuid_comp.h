#pragma once

#include <uuid.h>

namespace Components
{
    class UUID {
    public:
        UUID(const uuids::uuid&);
        UUID();

        uuids::uuid Id() const;

        bool Exists() const;
        operator bool() const;

    private:
        uuids::uuid m_uuid;
    };

} // Components

bool operator ==(Components::UUID const& lhs, Components::UUID const& rhs) noexcept;
bool operator ==(Components::UUID const& lhs, uuids::uuid const& rhs) noexcept;
bool operator ==(uuids::uuid const& lhs, Components::UUID const& rhs) noexcept;