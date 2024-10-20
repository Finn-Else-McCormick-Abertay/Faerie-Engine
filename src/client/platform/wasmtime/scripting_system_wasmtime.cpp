#include "scripting_system_wasmtime.h"

#include <systems/resource_manager.h>

#include <stdio.h>

bool ScriptingSystemWasmtime::InitImpl() {
    wasmtime::Config config;
    pm_engine = std::make_unique<wasmtime::Engine>(std::move(config));
    pm_store = std::make_unique<wasmtime::Store>(*pm_engine);

    return true;
};

void ScriptingSystemWasmtime::ShutdownImpl() {
    pm_engine = nullptr;
    pm_store = nullptr;
};

wasmtime::Engine& ScriptingSystemWasmtime::Engine() {
    auto& inst = dynamic_cast<ScriptingSystemWasmtime&>(Instance());
    return *inst.pm_engine;
}

wasmtime::Store& ScriptingSystemWasmtime::Store() {
    auto& inst = dynamic_cast<ScriptingSystemWasmtime&>(Instance());
    return *inst.pm_store;
}

wasmtime::Instance ScriptingSystemWasmtime::CreateInstance(const wasmtime::Module& module) {
    std::vector<wasmtime::Extern> imports;
    imports.push_back(wasmtime::Func::wrap(Store(), [](){ printf("Imported function called.\n"); }));
    auto result = wasmtime::Instance::create(Store(), module, imports);
    return result.unwrap();
}

void ScriptingSystemWasmtime::SetupExports(Script& script) {
    script.m_funcs.emplace("run", std::get<wasmtime::Func>(*script.m_instance.get(ScriptingSystemWasmtime::Store(), "run")));
}