#include "scripting_system_impl_wasmtime.h"

bool ScriptingSystemImplWasmtime::InitImpl() {
    pm_engine = std::make_unique<wasmtime::Engine>();

    return true;
};

void ScriptingSystemImplWasmtime::ShutdownImpl() {
    pm_engine = nullptr;
};