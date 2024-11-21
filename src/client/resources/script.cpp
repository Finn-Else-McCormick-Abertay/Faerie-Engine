#include "script.h"

#include <systems/resource_manager.h>
#include <systems/scripting_system.h>
#include <systems/logger.h>

#ifdef WASMTIME
#include <systems/platform/wasmtime/scripting_system_wasmtime.h>

Script::Script(wasmtime::Instance&& instance) : m_instance(std::move(instance)) {}

std::vector<wasmtime::Val> Script::Call(const std::string& func, const std::vector<wasmtime::Val>& args) {
    auto result = m_funcs.at(func).call(ScriptingSystemWasmtime::Store(), args);
    return result.unwrap();
}


template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>& info) {
    auto fileInfo = vfspp::FileInfo(info.Path());
    if (!fileInfo.IsValid()) {
        Logger::Error<Script>("Failed to load from ", info, ": path is invalid.");
        //return Script();
    }

    std::unique_ptr<wasmtime::Module> module = nullptr;

    // File is .wasm binary file
    if (fileInfo.Extension() == ".wasm") {
        std::vector<uint8_t> srcBytes = ReadBinaryFile(info.Path());
        if (auto result = wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcBytes)) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
        else { Logger::Error<Script>("Failed to load from ", info, ": script does not compile."); }
    }
    // File is .wat text file
    else if (fileInfo.Extension() == ".wat") {
        std::string srcText = ReadTextFile(info.Path());
        if (auto result = wasmtime::Module::compile(ScriptingSystemWasmtime::Engine(), srcText)) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
        else { Logger::Error<Script>("Failed to load from ", info, ": script does not compile."); }
    }
    else {
        Logger::Error<Script>("Failed to load from ", info, ": ", fileInfo.Extension(), " is not a valid WebAssembly script type.");
    }

    if (!module) {
        //return Script();
    }

    auto instance = ScriptingSystemWasmtime::CreateInstance(*module);
    Script script(std::move(instance));

    ScriptingSystemWasmtime::SetupExports(script);

    return script;
}
#endif // WASMTIME