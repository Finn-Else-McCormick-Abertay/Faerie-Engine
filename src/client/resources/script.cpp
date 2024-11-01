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

    std::unique_ptr<wasmtime::Module> module = nullptr;

    // File is .wasm binary file
    if (fileInfo.Extension() == ".wasm") {
        std::vector<uint8_t> srcBytes = ReadBinaryFile(info.Path());
        if (auto result = wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcBytes)) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
    }
    // File is .wat text file
    else if (fileInfo.Extension() == ".wat") {
        std::string srcText = ReadTextFile(info.Path());
        if (auto result = wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcText)) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
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