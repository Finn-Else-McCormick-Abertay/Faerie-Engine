#include "scripting_system.h"
#include <systems/system_lifecycle_define.h>

#ifdef WASMTIME
#include <systems/platform/wasmtime/scripting_system_wasmtime.h>
FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_POLYMORPHIC(ScriptingSystem, ScriptingSystemWasmtime)
#endif // WASMTIME

FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptingSystem)