#include "script_engine.h"
#include <systems/system_lifecycle_define.h>

#include <systems/resource_manager.h>
#include <systems/Logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ScriptEngine)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptEngine)

bool ScriptEngine::__Internal_Init() {
    wasmtime::Config config;
    pm_engine = std::make_unique<wasmtime::Engine>(std::move(config));
    pm_linker = std::make_unique<wasmtime::Linker>(*pm_engine);
    //pm_linker->define_wasi().unwrap();

    pm_store = std::make_unique<wasmtime::Store>(*pm_engine);
    //wasmtime::WasiConfig wasiConfig;
    //pm_store->context().set_wasi(std::move(wasiConfig)).unwrap();

    return true;
};

void ScriptEngine::__Internal_Shutdown() {};

wasmtime::Engine& ScriptEngine::Engine() {
    return *Instance().pm_engine;
}

wasmtime::Store& ScriptEngine::Store() {
    return *Instance().pm_store;
}

std::optional<wasmtime::Extern> ScriptEngine::GetExtern(const std::string& name) {
    auto& inst = Instance();
    if (inst.m_wrappedFuncs.contains(name)) {
        return inst.m_wrappedFuncs.at(name);
    }
    return std::nullopt;
}
