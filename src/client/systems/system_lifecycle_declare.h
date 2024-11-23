#pragma once

// To be placed at top of class declaration for system. Exits as private.

#define FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(SystemName)\
public:\
    static SystemName& Instance();\
private:\
    bool __internal_m_initialised = false;\
    SystemName() = default;\
    bool __Internal_Init();\
    void __Internal_Shutdown();\
public:\
    static bool Init();\
    static void Shutdown();\
private:

// To be placed at top of class declaration for abstract system. Exits as private.

#define FAERIE___SYSTEM_ABSTRACT_SINGLETON_LIFECYCLE_DECLARE(SystemName)\
public:\
    static SystemName& Instance();\
private:\
    bool __internal_m_initialised = false;\
protected:\
    SystemName() = default;\
    virtual bool __Internal_Init() = 0;\
    virtual void __Internal_Shutdown() = 0;\
public:\
    static bool Init();\
    static void Shutdown();\
private:

// To be placed at top of class declaration for derived system. Exits as private.

#define FAERIE___SYSTEM_POLYMORPHIC_LIFECYCLE_DECLARE(SystemName, ParentSystemName)\
private:\
    friend class ParentSystemName;\
    SystemName() = default;\
    virtual bool __Internal_Init() override;\
    virtual void __Internal_Shutdown() override;\
private:
