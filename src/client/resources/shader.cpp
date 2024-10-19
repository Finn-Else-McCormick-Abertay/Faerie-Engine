#include "shader.h"

#include <util/hash.h>

Shader::Shader(unsigned int programId) : m_programId(programId) {}

unsigned int Shader::ProgramId() { return m_programId; }


ResourceInfo<Shader>::ResourceInfo(const std::string& vertPath, const std::string& fragPath) : m_vert(vertPath), m_frag(fragPath) {}

ResourceIdentifier ResourceInfo<Shader>::Identifier() const {
    size_t hash = 0;
    util::hash_combine(hash, typeid(Shader).hash_code(), m_vert, m_frag);
    return hash;
}

const std::string& ResourceInfo<Shader>::VertPath() const { return m_vert; }
const std::string& ResourceInfo<Shader>::FragPath() const { return m_frag; }