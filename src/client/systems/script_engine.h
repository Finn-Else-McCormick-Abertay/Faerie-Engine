#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/script.h>

#include <resources/resource_info.h>
#include <memory>

#ifdef WASMTIME
// For some reason wasmtime.hh needs string and iostream included before it?
#include <string>
#include <iostream>
#include <wasmtime.hh>
#endif // WASMTIME

class ScriptEngine {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ScriptEngine)
public:



#ifdef WASMTIME
protected:
    static wasmtime::Engine& Engine();
    static wasmtime::Store& Store();

    static wasmtime::Instance CreateInstance(const wasmtime::Module&);
    static void SetupExports(Script&);

    friend class ResourceManager;
    friend class Script;

private:
    std::unique_ptr<wasmtime::Engine> pm_engine;
    std::unique_ptr<wasmtime::Linker> pm_linker;
    std::unique_ptr<wasmtime::Store> pm_store;
#endif // WASMTIME
};