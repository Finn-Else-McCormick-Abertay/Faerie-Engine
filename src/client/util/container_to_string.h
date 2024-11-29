#pragma once

#include <string>
#include <sstream>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>

template<typename T>
std::string vector_to_string(const std::vector<T>& vec) {
    std::stringstream ss;
    ss << "[";
    bool first = true;
    for (auto& val : vec) {
        if (!first) { ss << ", "; } first = false;
        ss << val;
    }
    ss << "]";
    return ss.str();
}

template<typename T>
std::string set_to_string(const std::set<T>& set) {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (auto& val : set) {
        if (!first) { ss << ", "; } first = false;
        ss << val;
    }
    ss << "}";
    return ss.str();
}

template<typename K, typename V>
std::string map_to_string(const std::map<K, V>& map) {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (auto& [k, v] : map) {
        if (!first) { ss << ", "; } first = false;
        ss << "(" << k << ": " << v << ")";
    }
    ss << "}";
    return ss.str();
}

template<typename K, typename V>
std::string map_to_string(const std::unordered_map<K, V>& map) {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (auto& [k, v] : map) {
        if (!first) { ss << ", "; } first = false;
        ss << "(" << k << ": " << v << ")";
    }
    ss << "}";
    return ss.str();
}