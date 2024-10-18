#include "scripting_system.h"

#include <memory>
#ifdef WASMTIME
#include <systems/impl/scripting_system_impl_wasmtime.h>
#endif // WASMTIME

ScriptingSystem& ScriptingSystem::Instance() {
#ifdef WASMTIME
    static std::unique_ptr<ScriptingSystem> inst = std::unique_ptr<ScriptingSystem>(new ScriptingSystemImplWasmtime());
#endif // WASMTIME
    return *inst;
}