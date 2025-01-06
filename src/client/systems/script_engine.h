#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/resource_info.h>
#include <memory>
#include <unordered_map>

#include <cxx.h>
#include <util/cxx_ergonomics.h>
#include <faerie_rust_bindings.h>

#include <maths_types.h>

class ScriptEngine {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ScriptEngine)
public:
    static faerie_rust::ScriptEngine& Engine();

    static void Process(double delta);
    static void OnInput(const std::string& actionName, bool isPressed);
    static void OnInput(const std::string& actionName, const vec2& motion);

private:
    OptionalBox<faerie_rust::ScriptEngine> pm_engine;
};