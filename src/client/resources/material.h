#pragma once

#include <resources/resource_info.h>
#include <resources/texture.h>
#include <resources/shader.h>

class Material
{
public:
    Material(ResourceIdentifier shader);

    ResourceIdentifier ShaderId() const;

private:
    ResourceIdentifier m_shader;
};