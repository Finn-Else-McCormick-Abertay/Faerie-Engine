#include "script.h"

#include <systems/resource_manager.h>
#include <systems/script_engine.h>
#include <systems/logger.h>

Script::Script(OptionalBox<faerie_rust::Script>&& script) : pm_script(std::move(script)) {}
Script::Script() : pm_script() {}

template<> Script ResourceManager::__LoadInternal(const ResourceInfo<Script>& info) {
    auto fileInfo = vfspp::FileInfo(info.Path());
    if (!fileInfo.IsValid()) {
        Logger::Error<Script>("Failed to load from ", info, ": path is invalid.");
        return Script();
    }
    if (fileInfo.Extension() != ".wasm" && fileInfo.Extension() != ".wat") {
        Logger::Error<Script>("Failed to load from ", info, ": ", fileInfo.Extension(), " is not a valid WASM file type.");
        return Script();
    }

    // This works with the text format too, it just gets it as raw bytes
    auto bytes = ReadBinaryFile(info.Path());

    OptionalBox<faerie_rust::Script> rustScript;
    try {
        rustScript = OptionalBox<faerie_rust::Script>(ScriptEngine::Engine().load_module(rust::Slice<const uint8_t>(bytes)));
    }
    catch (rust::Error& err) {
        Logger::Error<Script>("Failed to load from ", info, " due to Rust error: ", err.what());
        return Script();
    }

    // Module compiled successfully

    return Script(std::move(rustScript));

    /*
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