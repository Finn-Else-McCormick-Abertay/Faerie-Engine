#pragma once

#include <systems/scripting_system.h>

#include <resources/script.h>

// For some reason wasmtime.hh needs string and iostream included before it?
#include <string>
#include <iostream>
#include <wasmtime.hh>

#include <memory>

class ScriptingSystemWasmtime : public ScriptingSystem
{
public:

protected:
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    static wasmtime::Engine& Engine();
    static wasmtime::Store& Store();

    static wasmtime::Instance CreateInstance(const wasmtime::Module&);
    static void SetupExports(Script&);

    friend class ResourceManager;
    friend class Script;

private:
    std::unique_ptr<wasmtime::Engine> pm_engine;
    std::unique_ptr<wasmtime::Store> pm_store;
};