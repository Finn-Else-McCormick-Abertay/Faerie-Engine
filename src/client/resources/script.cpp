#include "script.h"

#include <systems/resource_manager.h>
#include <systems/scripting_system.h>

#ifdef WASMTIME
#include <platform/wasmtime/scripting_system_wasmtime.h>

Script::Script(wasmtime::Instance&& instance) : m_instance(std::move(instance)) {}

std::vector<wasmtime::Val> Script::Call(const std::string& func, const std::vector<wasmtime::Val>& args) {
    auto result = m_funcs.at(func).call(ScriptingSystemWasmtime::Store(), args);
    return result.unwrap();
}


template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>& info) {
    auto fileInfo = vfspp::FileInfo(info.Path());
    if (!fileInfo.IsValid()) {
        // do something???
        //return Script();
    }

    std::unique_ptr<wasmtime::Module> module;

    // File is .wasm binary file
    if (fileInfo.Extension() == ".wasm") {
        std::span<uint8_t> srcBytes = ReadBinaryFile(info.Path());
        auto result = wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcBytes);
        if (result) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
        else {
            module = nullptr;
        }
    }
    // File is .wat text file
    else if (fileInfo.Extension() == ".wat") {
        std::string srcText = ReadTextFile(info.Path());
        module = std::make_unique<wasmtime::Module>(std::move(wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcText).unwrap()));
    }
    else {
        // Fail in some kind of way
    }

    auto instance = ScriptingSystemWasmtime::CreateInstance(*module);
    Script script(std::move(instance));

    ScriptingSystemWasmtime::SetupExports(script);

    return script;
}

template<> void ResourceManager::__UnloadInternal(Script& script) {
}
#endif // WASMTIME