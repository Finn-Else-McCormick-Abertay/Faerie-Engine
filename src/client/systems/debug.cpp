#include "debug.h"
#include <systems/system_lifecycle_define.h>

#include <systems/window_system.h>
#include <systems/logger.h>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(Debug)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(Debug)

bool Debug::__Internal_Init() {
    return true;
}

void Debug::__Internal_Shutdown() {}

void Debug::SetPersistent(const std::string& identifier, std::function<void()>&& callback) {
    if (Instance().m_persistentCallbacks.contains(identifier)) {
        Logger::Debug<Debug>("Debug callback '", identifier, "' overriden.");
    }
    Instance().m_persistentCallbacks.insert_or_assign(identifier, std::move(callback));
}

void Debug::ClearPersistent(const std::string& identifier) {
    Instance().m_persistentCallbacks.erase(identifier);
}

void Debug::PushInstant(std::function<void()>&& callback) {
    Instance().m_instantCallbackQueue.push(std::move(callback));
}

void Debug::__Internal_RunImGuiCallbacks() {
    while (!m_instantCallbackQueue.empty()) {
        (m_instantCallbackQueue.front())();
        m_instantCallbackQueue.pop();
    }
    for (auto& [id, callback] : m_persistentCallbacks) { callback(); }
}