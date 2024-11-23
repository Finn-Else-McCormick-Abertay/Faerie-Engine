#pragma once

// This is a separate header from the declaration macros as logger obviously can't be included there
#include <systems/logger.h>
#include <memory>

// To be placed in the cpp file. Creates the base class itself.
#define FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(SystemName)\
SystemName& SystemName::Instance() { static SystemName inst; return inst; }

// To be placed in the cpp file of the base class. Creates a derivative of the class, returns it as the class.
#define FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_POLYMORPHIC(SystemName, DerivedSystemName)\
SystemName& SystemName::Instance() { static std::unique_ptr<SystemName> uptr = std::unique_ptr<SystemName>(new DerivedSystemName()); return *uptr; }

// To be placed in the cpp file. Sets up static init and shutdown methods; must be followed by definitions of __Internal_Init and __Internal_Shutdown.
#define FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(SystemName)\
bool SystemName::Init() {\
    auto& inst = Instance();\
    if (!inst.__internal_m_initialised) {\
        inst.__internal_m_initialised = inst.__Internal_Init();\
        if (inst.__internal_m_initialised) { Logger::Info<SystemName>("Initialised"); }\
        else { Logger::Error<SystemName>("Failed to initialise"); }\
        return inst.__internal_m_initialised; \
    }\
    else {\
        Logger::Warning(inst, "Init called again on already-initialised system");\
        return false;\
    }\
}\
void SystemName::Shutdown() {\
    auto& inst = Instance();\
    if (inst.__internal_m_initialised) {\
        Logger::Info<SystemName>("Shutting down");\
        inst.__Internal_Shutdown();\
    }\
    else { Logger::Warning<SystemName>("Shutdown called on uninitialised system"); }\
}

#define FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_IMPL_DEFINE_EMPTY(SystemName)\
bool SystemName::__Internal_Init() { return true; }\
void SystemName::__Internal_Shutdown() {}
