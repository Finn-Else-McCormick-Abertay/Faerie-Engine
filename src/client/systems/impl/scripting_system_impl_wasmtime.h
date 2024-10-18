#pragma once

#include <systems/scripting_system.h>

#include <memory>

// For some reason wasmtime.hh needs string and iostream included before it?
// This and the wrong int type thing are. Not good signs
#include <string>
#include <iostream>
#include <wasmtime.hh>

class ScriptingSystemImplWasmtime : public ScriptingSystem
{
public:

protected:
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

private:
    std::unique_ptr<wasmtime::Engine> pm_engine;
};