#include "script_engine.h"
#include <systems/system_lifecycle_define.h>

#include <systems/resource_manager.h>
#include <systems/Logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ScriptEngine)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptEngine)

#ifdef WASMTIME

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

wasmtime::Instance ScriptEngine::CreateInstance(const wasmtime::Module& module) {
    for (auto& import : module.imports()) {
        //?????
    }
    std::vector<wasmtime::Extern> imports;
    imports.push_back(wasmtime::Func::wrap(Store(), [](){ printf("Imported function called.\n"); }));
    auto result = wasmtime::Instance::create(Store(), module, imports);
    return result.unwrap();
}

void ScriptEngine::SetupExports(Script& script) {
    script.m_funcs.emplace("run", std::get<wasmtime::Func>(*script.m_instance.get(Store(), "run")));
}

#endif // WASMTIME
