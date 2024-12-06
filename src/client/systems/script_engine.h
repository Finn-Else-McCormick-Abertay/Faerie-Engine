#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/resource_info.h>
#include <memory>
#include <unordered_map>

// For some reason wasmtime.hh needs string and iostream included before it?
#include <string>
#include <iostream>
#include <wasmtime.hh>

#include <rust/cxx.h>
#include <faerie_rust/src/lib.rs.h>

class ScriptEngine {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ScriptEngine)
public:

private:
    std::unique_ptr<rust::Box<faerie_rust::ScriptEngine>> pm_engine;
};