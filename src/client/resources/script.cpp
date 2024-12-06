#include "script.h"

#include <systems/resource_manager.h>
#include <systems/script_engine.h>
#include <systems/logger.h>

Script::Script(wasmtime::Instance&& instance) {}//: pm_instance(std::make_unique<wasmtime::Instance>(std::move(instance))) {}
Script::Script() {}//: pm_instance(nullptr) {}

/*
std::vector<wasmtime::Val> Script::Call(const std::string& func, const std::vector<wasmtime::Val>& args) {
    try {
        auto result = m_funcs.at(func).call(ScriptEngine::Store(), args);
        return result.unwrap();
    } catch (const std::out_of_range& err) {
        Logger::Error(*this, "Attempted to call non-existent function '", func, "'");
        return std::vector<wasmtime::Val>{};
    }
}
*/

template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>& info) {
    /*
    auto fileInfo = vfspp::FileInfo(info.Path());
    if (!fileInfo.IsValid()) {
        Logger::Error<Script>("Failed to load from ", info, ": path is invalid.");
        return Script();
    }

    std::unique_ptr<wasmtime::Module> module = nullptr;

    auto outputCompileError = [&info](const std::string& message, const wasmtime::Trace& trace) {
        std::string frameTrace = "";
        int index = 0;
        for (auto& frame : trace) {
            if (index > 0) { frameTrace += "\n"; }
            for (int i = 0; i < index - 1; ++i) { frameTrace += "  "; }
            if (index > 0) { frameTrace += " -"; }
            frameTrace += "[" + std::to_string(index) + "] ";
            frameTrace += " ("; frameTrace += frame.module_name().value_or("unnamed module"); frameTrace += ")";
            frameTrace += " : " + std::to_string(frame.module_offset());
            frameTrace += " -> func ("; frameTrace += frame.func_name().value_or("unnamed func"); frameTrace += ")";
            frameTrace += " : " + std::to_string(frame.func_offset());
            index++;
        }
        Logger::Error<Script>("Failed to compile while loading from ", info, ": ", message, "\n", frameTrace);
    };

    // File is .wasm binary file
    if (fileInfo.Extension() == ".wasm") {
        std::vector<uint8_t> srcBytes = ReadBinaryFile(info.Path());
        auto result = wasmtime::Module::compile(ScriptEngine::Engine(), srcBytes);
        if (result) { module = std::make_unique<wasmtime::Module>(std::move(result.ok())); }
        else {
            outputCompileError(result.err().message(), result.err().trace());
            return Script();
        }
    }
    // File is .wat text file
    else if (fileInfo.Extension() == ".wat") {
        std::string srcText = ReadTextFile(info.Path());
        auto result = wasmtime::Module::compile(ScriptEngine::Engine(), srcText);
        if (result) { module = std::make_unique<wasmtime::Module>(std::move(result.ok())); }
        else {
            outputCompileError(result.err().message(), result.err().trace());
            return Script();
        }
    }
    else {
        Logger::Error<Script>("Failed to load from ", info, ": ", fileInfo.Extension(), " is not a valid WebAssembly script type.");
        return Script();
    }

    // Module compiled successfully

    // Get requested imports
    std::vector<wasmtime::Extern> importExterns;
    for (auto importType : module->imports()) {
        if (importType.module() == "env") {
            auto ext = ScriptEngine::GetExtern(std::string(importType.name()));
            if (ext) { importExterns.push_back(ext.value()); }
            else {
                Logger::Error<Script>("Could not provide import '(",  importType.module(), ")::", importType.name(), "' while loading from ", info);
                // Could also assign it to an empty function? idk
                return Script();
            }
        }
        else {
            Logger::Warning<Script>("Attempted to import non-host function '(", importType.module(), ")::", importType.name(), "' (not currently implemented)");
        }
    }

    // Instantiate module
    Script script;
    auto result = wasmtime::Instance::create(ScriptEngine::Store(), *module, importExterns);
    if (result) { script = Script(std::move(result.ok())); }
    else {
        Logger::Error<Script>("Failed to instantiate while loading from ", info, ": ", result.err().message());
        return Script();
    }

    // Add exports to script object
    //for (auto exportType : module->exports()) {
    //    Logger::Debug<Script>(info, " export: ", exportType.name());
    //}
    script.m_funcs.emplace("run", std::get<wasmtime::Func>(*script.pm_instance->get(ScriptEngine::Store(), "run")));
    script.m_funcs.emplace("number", std::get<wasmtime::Func>(*script.pm_instance->get(ScriptEngine::Store(), "number")));

    return script;
    */
}