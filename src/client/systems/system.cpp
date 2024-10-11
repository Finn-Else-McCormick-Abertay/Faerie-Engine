#include "system.h"

bool ISystem::Init() {
    if (!m_initialised) {
        m_initialised = InitImpl();
    }
    return m_initialised;
}

void ISystem::Shutdown() {
    if (m_initialised) {
        ShutdownImpl();
        m_initialised = false;
    }
}

bool ISystem::Initialised() const {
    return m_initialised;
}