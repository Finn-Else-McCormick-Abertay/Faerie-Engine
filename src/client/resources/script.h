#pragma once

#include <cxx.h>
#include <util/cxx_ergonomics.h>
#include <faerie_rust_bindings.h>

#include <unordered_map>
#include <string>
#include <memory>

class Script
{
public:
    bool Exists() const;
    faerie_rust::Script& Handle();

private:
    Script(OptionalBox<faerie_rust::Script>&&);
    Script();

    OptionalBox<faerie_rust::Script> pm_script;

    friend class ScriptEngine;
    friend class ResourceManager;
};