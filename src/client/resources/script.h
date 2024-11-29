#pragma once

#include <wasmtime.hh>

#include <unordered_map>
#include <string>
#include <memory>

class Script
{
public:
    std::vector<wasmtime::Val> Call(const std::string& func, const std::vector<wasmtime::Val>& args);

private:
    Script(wasmtime::Instance&&);
    Script();

    std::unique_ptr<wasmtime::Instance> pm_instance;
    std::unordered_map<std::string, wasmtime::Func> m_funcs;

    friend class ScriptEngine;
    friend class ResourceManager;
};