#include "json_archive.h"
#include <sstream>
#include <json5cpp.h>

std::string JsonOutputArchive::ToString() const {
	std::stringstream ss;
	Json5::serialize(ss, m_root);
	return ss.str();
}

void JsonOutputArchive::operator()(entt::entity ent) {
	Json::Value val;
	val["id"] = static_cast<std::underlying_type_t<entt::entity>>(ent);
	m_root["entities"].append(val);
}

void JsonOutputArchive::operator()(std::underlying_type_t<entt::entity> v) {
	m_root["size"] = v;
}