#include "script.h"

#include <systems/resource_manager.h>
#include <systems/script_engine.h>
#include <systems/logger.h>

#ifdef WASMTIME

Script::Script(wasmtime::Instance&& instance) : m_instance(std::move(instance)) {}

std::vector<wasmtime::Val> Script::Call(const std::string& func, const std::vector<wasmtime::Val>& args) {
    try {
        auto result = m_funcs.at(func).call(ScriptEngine::Store(), args);
        return result.unwrap();
    } catch (const std::out_of_range& err) {
        Logger::Error(*this, "Attempted to call non-existent function '", func, "'");
        return std::vector<wasmtime::Val>{};
    }
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
        if (auto result = wasmtime::Module::compile(ScriptEngine::Engine(), srcBytes)) {
            module = std::make_unique<wasmtime::Module>(std::move(result.ok()));
        }
        else { Logger::Error<Script>("Failed to load from ", info, ": script does not compile."); }
    }
    // File is .wat text file
    else if (fileInfo.Extension() == ".wat") {
        std::string srcText = ReadTextFile(info.Path());
        if (auto result = wasmtime::Module::compile(ScriptEngine::Engine(), srcText)) {
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

    auto instance = ScriptEngine::CreateInstance(*module);
    Script script(std::move(instance));

    ScriptEngine::SetupExports(script);

    return script;
}
#endif // WASMTIME