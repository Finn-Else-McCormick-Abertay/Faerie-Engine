#include "script_engine.h"
#include <systems/system_lifecycle_define.h>

#include <systems/resource_manager.h>
#include <systems/Logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ScriptEngine)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptEngine)

bool ScriptEngine::__Internal_Init() {
    pm_engine = std::unique_ptr<rust::Box<faerie_rust::ScriptEngine>>(new rust::Box<faerie_rust::ScriptEngine>(faerie_rust::create_script_engine()));
    //(**pm_engine);

    
    return true;
};

void ScriptEngine::__Internal_Shutdown() {};