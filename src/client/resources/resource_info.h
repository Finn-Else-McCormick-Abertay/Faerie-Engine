#pragma once

#include <string>
#include <typeinfo>
#include <util/hash.h>

using ResourceIdentifier = size_t;

template<typename T>
class ResourceInfo
{
public:
    ResourceInfo(const std::string& path) : m_path(path) {}

    ResourceIdentifier Identifier() const {
        size_t hash = 0;
        util::hash_combine(hash, typeid(T).hash_code(), m_path);
        return hash;
    }

    std::string Path() const {
        return m_path;
    }

private:
    std::string m_path;
};