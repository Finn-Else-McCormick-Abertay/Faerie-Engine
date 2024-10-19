#pragma once

#include <resources/resource_info.h>

class Shader
{
public:
    Shader(unsigned int programId);

    unsigned int ProgramId();

private:
    unsigned int m_programId;
};


template<>
class ResourceInfo<Shader>
{
public:
    ResourceInfo(const std::string& vertPath, const std::string& fragPath);

    ResourceIdentifier Identifier() const;

    const std::string& VertPath() const;
    const std::string& FragPath() const;
private:
    std::string m_vert, m_frag;
};