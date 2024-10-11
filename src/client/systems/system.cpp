#include "system.h"

ISystem::~ISystem() {
    Shutdown();
}

bool ISystem::Init() {
    if (m_initialised) {
        return false;
    }
    m_initialised = InitImpl();
    return m_initialised;
}

void ISystem::Shutdown() {
    if (m_initialised) {
        ShutdownImpl();
        m_initialised = false;
    }
}