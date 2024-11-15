#include "uuid_comp.h"

Components::UUID::UUID(const uuids::uuid& uuid) : m_uuid(std::move(uuid)) {}
Components::UUID::UUID() : UUID(uuids::uuid()) {}

uuids::uuid Components::UUID::Id() const { return m_uuid; }

bool Components::UUID::Exists() const { return !m_uuid.is_nil(); }

Components::UUID::operator bool() const { return Exists(); }

bool operator ==(Components::UUID const& lhs, Components::UUID const& rhs) noexcept { return lhs.Id() == rhs.Id(); }
bool operator ==(Components::UUID const& lhs, uuids::uuid const& rhs) noexcept { return lhs.Id() == rhs; }
bool operator ==(uuids::uuid const& lhs, Components::UUID const& rhs) noexcept { return lhs == rhs.Id(); }