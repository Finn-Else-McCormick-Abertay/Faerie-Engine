#include "scripting_system.h"

#include <memory>
#ifdef WASMTIME
#include <platform/wasmtime/scripting_system_wasmtime.h>
#endif // WASMTIME

ScriptingSystem& ScriptingSystem::Instance() {
    static std::unique_ptr<ScriptingSystem> inst = std::unique_ptr<ScriptingSystem>(
#ifdef WASMTIME
        new ScriptingSystemWasmtime()
#endif // WASMTIME
    );
    return *inst;
}