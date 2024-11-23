#include "script_engine.h"
#include <systems/system_lifecycle_define.h>

#ifdef WASMTIME
#include <systems/platform/script_engine_wasmtime.h>
FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_POLYMORPHIC(ScriptEngine, ScriptingSystemWasmtime)
#endif // WASMTIME

FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptEngine)