#pragma once

#include <entt/entity/entity.hpp>
#include <json/json.h>
#include <string>

class JsonOutputArchive
{
public:
    JsonOutputArchive() = default;

    std::string ToString() const;

    void operator()(entt::entity);
    void operator()(std::underlying_type_t<entt::entity>);

    template<typename T>
    void operator()(const T& component) {
        m_root["components"].append(1);
    }

private:
    Json::Value m_root;
};