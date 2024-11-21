#pragma once

#include <vector>

class Texture
{
public:
    Texture();
    Texture(const std::vector<uint8_t>& data);
    Texture(Texture&&);
    Texture(const Texture&) = delete;
    ~Texture();

    unsigned int TextureId();

private:
    unsigned int m_buffer;
};