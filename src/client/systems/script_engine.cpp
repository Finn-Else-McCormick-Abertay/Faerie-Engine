#include "script_engine.h"
#include <systems/system_lifecycle_define.h>

#include <systems/resource_manager.h>
#include <systems/logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(ScriptEngine)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(ScriptEngine)

bool ScriptEngine::__Internal_Init() {
    pm_engine = OptionalBox<faerie_rust::ScriptEngine>(faerie_rust::create_script_engine());
    
    return true;
};

void ScriptEngine::__Internal_Shutdown() {};

faerie_rust::ScriptEngine& ScriptEngine::Engine() {
    return *Instance().pm_engine;
}

void ScriptEngine::Process(double delta) {
    ResourceManager::ForEach<Script>([&delta](ResourceIdentifier id, Script& script){
        if (script.Exists()) {
            script.Handle().process(delta);
        }
    });
}

void ScriptEngine::OnInput(const std::string& actionName, bool isPressed) {
    auto rustStr = rust::Str(actionName);
    ResourceManager::ForEach<Script>([&rustStr, &isPressed](ResourceIdentifier id, Script& script){
        if (script.Exists()) {
            script.Handle().on_input_press(rustStr, isPressed);
        }
    });
}

void ScriptEngine::OnInput(const std::string& actionName, const vec2& motion) {
    auto rustStr = rust::Str(actionName);
    ResourceManager::ForEach<Script>([&rustStr, &motion](ResourceIdentifier id, Script& script){
        if (script.Exists()) {
            script.Handle().on_input_motion(rustStr, motion.x, motion.y);
        }
    });
}