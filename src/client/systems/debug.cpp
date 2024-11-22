#include "debug.h"

#include <systems/window_system.h>
#include <systems/logger.h>

Debug& Debug::Instance() {
    static Debug debug;
    return debug;
}

bool Debug::InitImpl() {
    return true;
}

void Debug::ShutdownImpl() {}

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
    for (auto [id, callback] : m_persistentCallbacks) { callback(); }
}