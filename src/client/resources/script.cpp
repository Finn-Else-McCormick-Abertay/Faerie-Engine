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
        rustScript = OptionalBox<faerie_rust::Script>(ScriptEngine::Engine().load_script(rust::Slice<const uint8_t>(bytes)));
    }
    catch (rust::Error& err) {
        Logger::Error<Script>("Failed to load from ", info, ": ", err.what());
        return Script();
    }

    rustScript->test();

    return Script(std::move(rustScript));
}