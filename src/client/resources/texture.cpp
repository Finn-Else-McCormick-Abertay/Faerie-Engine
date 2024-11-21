#include "texture.h"

#include <systems/resource_manager.h>
#include <systems/logger.h>

#include <util/stb_image.h>

Texture::Texture(const std::vector<uint8_t>& data) {
    int width, height, channels;
    unsigned short* image = stbi_load_16_from_memory(data.data(), data.size() * sizeof(uint8_t), &width, &height, &channels, STBI_rgb_alpha);

    glGenTextures(1, &m_buffer);
    glBindTexture(GL_TEXTURE_2D, m_buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, image);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

Texture::Texture() {
    m_buffer = 0;    
}

Texture::Texture(Texture&& rhs) : m_buffer(rhs.m_buffer) {
    rhs.m_buffer = 0;    
}

Texture::~Texture() {
    if (m_buffer) { glDeleteTextures(1, &m_buffer); }
}

unsigned int Texture::TextureId() { return m_buffer; }

template<>
Texture ResourceManager::__LoadInternal(const ResourceInfo<Texture>& info) {
    auto data = ReadBinaryFile(info.Path());
    return Texture(data);
}