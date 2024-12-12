#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/resource_info.h>
#include <memory>
#include <unordered_map>

#include <cxx.h>
#include <util/cxx_ergonomics.h>
#include <faerie_rust_bindings.h>

class ScriptEngine {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ScriptEngine)
public:
    static const faerie_rust::ScriptEngine& Engine();

private:
    OptionalBox<faerie_rust::ScriptEngine> pm_engine;
};