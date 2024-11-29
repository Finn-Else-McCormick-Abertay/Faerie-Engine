#pragma once

#include <systems/system_lifecycle_declare.h>

#include <resources/resource_info.h>
#include <memory>
#include <unordered_map>

// For some reason wasmtime.hh needs string and iostream included before it?
#include <string>
#include <iostream>
#include <wasmtime.hh>

class ScriptEngine {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(ScriptEngine)
public:
    static std::optional<wasmtime::Extern> GetExtern(const std::string&);
    
    template <typename F,
            std::enable_if_t<wasmtime::WasmHostFunc<F>::Params::valid, bool> = true,
            std::enable_if_t<wasmtime::WasmHostFunc<F>::Results::valid, bool> = true>
    static void SetFunc(const std::string& name, F f) {
        Instance().m_wrappedFuncs.emplace(name, wasmtime::Func::wrap(Store(), std::forward<F>(f)));
    }

protected:
    static wasmtime::Engine& Engine();
    static wasmtime::Store& Store();

    friend class ResourceManager;
    friend class Script;

private:
    std::unique_ptr<wasmtime::Engine> pm_engine;
    std::unique_ptr<wasmtime::Linker> pm_linker;
    std::unique_ptr<wasmtime::Store> pm_store;

    std::unordered_map<std::string, wasmtime::Func> m_wrappedFuncs;
};