#include "json_archive.h"
#include <sstream>
#include <json5cpp.h>

std::string JsonOutputArchive::ToString() const {
	std::stringstream ss;
	auto json = ToJson();
	Json5::SerializeConfig config {};
	config.indent = "    ";
	Json5::serialize(ss, json, config);
	return ss.str();
}

JsonInputArchive::JsonInputArchive(const std::string& json_string) {
	std::stringstream ss; ss << json_string;
	Json::Value root;
	Json5::ParseConfig config {};
	config.newlinesAsCommas = true;
	std::string err;
	if(!Json5::parse(ss, root, &err, config)) { Logger::Error<JsonInputArchive>(err); }

	for (auto& json : root["entities"]) { AddToMap(json); }
	//p_currentRoot= &m_root["entities"];
}