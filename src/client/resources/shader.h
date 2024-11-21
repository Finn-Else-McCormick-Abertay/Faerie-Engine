#pragma once

#include <resources/resource_info.h>
#include <iostream>

class Shader
{
public:
    Shader(unsigned int programId);
    Shader(Shader&&);
    Shader(const Shader&) = delete;
    ~Shader();

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

    friend std::ostream& operator<<(std::ostream& os, const ResourceInfo<Shader>& shader) {
        os << "Frag: " << shader.FragPath() << ", Vert" << shader.VertPath();
        return os;
    }
};