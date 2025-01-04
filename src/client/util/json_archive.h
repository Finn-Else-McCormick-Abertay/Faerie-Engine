#pragma once

#include <entt/entity/entity.hpp>
#include <json/json.h>
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <components/component_serialisation.h>
#include <components/hierarchy.h>
#include <systems/logger.h>

class JsonOutputArchive
{
public:
    JsonOutputArchive() = default;

    std::string ToString() const;

    inline Json::Value ToJson() const {
        Json::Value root;
        for (auto& orphan : m_orphans) { root["entities"].append(GetEntityJson(orphan)); }
        return root;
    }

    inline void operator()(entt::entity ent) {
        m_currentEntity = ent;
        if (!m_entityData.contains(ent)) { m_entityData.emplace(ent, std::make_pair(Json::Value(), std::vector<entt::entity>())); }
        m_entityData.at(ent).first["id"] = (unsigned int)ent;
    }
    inline void operator()(std::underlying_type_t<entt::entity> size) {}

    template<typename T> void operator()(const T& component) {
        if constexpr (has_serialise<T>::value) { component.__Serialise(m_entityData.at(m_currentEntity).first); }
        
        if (typeid(T) == typeid(Components::Hierarchy)) {
            auto& hierarchy = (const Components::Hierarchy&)component;
            hierarchy.ForEachChild([&](Entity entity){
                m_entityData.at(m_currentEntity).second.push_back(entity.Id());
            });
            if (!hierarchy.Parent().Exists()) { m_orphans.insert(m_currentEntity); }
        }
    }

private:
    std::unordered_map<entt::entity, std::pair<Json::Value, std::vector<entt::entity>>> m_entityData;
    std::set<entt::entity> m_orphans;
    entt::entity m_currentEntity;

    inline Json::Value GetEntityJson(entt::entity ent) const {
        if (!m_entityData.contains(ent)) { return Json::Value(); }
        
        auto& data = m_entityData.at(ent);
        Json::Value entJson = data.first;
        for (auto& child : data.second) { entJson["children"].append(GetEntityJson(child)); }
        return entJson;
    }
};

class JsonInputArchive
{
public:
    JsonInputArchive(const std::string& json_string);

    inline void operator()(entt::entity& ent) {
        Logger::Debug<JsonInputArchive>("ENT ", (unsigned int)ent);
        ent = (entt::entity)p_currentRoot->get(m_index, Json::Value())["id"].asUInt64();
        m_currentEntity = ent;
        ++m_index;
        int i = 0; for (auto& v : *p_currentRoot) { ++i; }
        if (m_index > i) {

        }
    }
    inline void operator()(std::underlying_type_t<entt::entity>& size) {
        int i = 0; for (auto& v : *p_currentRoot) { ++i; }
        size = i;
    }

    template<typename T> void operator()(T& component) {
        Logger::Debug<JsonInputArchive>("COMP");
        //const Json::Value& val = m_root["entities"][std::to_string((size_t)m_currentEntity)];
        //if constexpr (has_deserialise<T>::value) { component.__Deserialise(val); }
        //else { Logger::Warning<T>("Attempted to deserialise, but has no deserialise function."); }
    }

private:
    std::unordered_map<entt::entity, Json::Value> m_entityJson;
    std::vector<Json::Value> m_roots;
    Json::Value* p_currentRoot;
    entt::entity m_currentEntity;
    int m_index = 0;

    inline void AddToMap(Json::Value entityJson) {
		m_entityJson[(entt::entity)entityJson["id"].asUInt64()] = entityJson;
		if (!entityJson.get("children", Json::Value()).empty()) {
			for (auto& childJson : entityJson["children"]) { AddToMap(childJson); }
		}
    }
};