#include "material.h"

Material::Material(ResourceIdentifier shader) : m_shader(shader) {}

ResourceIdentifier Material::ShaderId() const { return m_shader; }