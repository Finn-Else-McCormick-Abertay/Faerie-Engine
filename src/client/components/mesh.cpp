#include "mesh.h"

Components::Mesh::Mesh(ResourceIdentifier meshId) : m_meshId(meshId) {}

ResourceIdentifier Components::Mesh::MeshId() const { return m_meshId; }