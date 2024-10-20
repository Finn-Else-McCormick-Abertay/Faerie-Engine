#pragma once

#ifdef WASMTIME
#include <wasmtime.hh>
#endif // WASMTIME

#include <unordered_map>
#include <string>

class Script
{
public:

#ifdef WASMTIME
    std::vector<wasmtime::Val> Call(const std::string& func, const std::vector<wasmtime::Val>& args);

protected:
    Script(wasmtime::Instance&&);

    wasmtime::Instance m_instance;

private:
    //std::vector<wasmtime::Val> Call(const std::string& func, const std::vector<wasmtime::Val>& args);

    std::unordered_map<std::string, wasmtime::Func> m_funcs;

    friend class ScriptingSystemWasmtime;
    
#endif // WASMTIME

private:
    friend class ResourceManager;
};