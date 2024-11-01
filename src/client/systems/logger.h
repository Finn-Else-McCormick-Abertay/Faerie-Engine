#pragma once

#include <systems/system.h>

#include <string>
#include <sstream>

#define _LOGGER_FUNCTION(FuncName)\
static void FuncName (const std::string& identifier, const std::string& message) { Instance().Output(identifier, message, OutputMode::FuncName); }\
static void FuncName (const ISystem& system, const std::string& message) { FuncName (system.Name(), message); }\
template<typename... Args> static void FuncName (const std::string& identifier, Args... args) { std::stringstream ss; ((ss << args), ...); FuncName (identifier, ss.str()); }\
template<typename... Args> static void FuncName (const ISystem& system, Args... args) { std::stringstream ss; ((ss << args), ...); FuncName (system, ss.str()); }

#define _LOGGER_FUNCTION_EMPTY(FuncName)\
static void FuncName (const std::string& identifier, const std::string& message) {}\
static void FuncName (const ISystem& system, const std::string& message) {}\
template<typename... Args> static void FuncName (const std::string& identifier, Args... args) {}\
template<typename... Args> static void FuncName (const ISystem& system, Args... args) {}

class Logger final : public ISystem
{
public:
    SYSTEM_LOGGER_NAME(Logger)

    static Logger& Instance();

    _LOGGER_FUNCTION(Info)
    _LOGGER_FUNCTION(Error)
    _LOGGER_FUNCTION(Warning)

#ifdef DEBUG
    _LOGGER_FUNCTION(Debug)
#endif
#ifndef DEBUG
    _LOGGER_FUNCTION_EMPTY(Debug)
#endif

private:
    Logger() = default;
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    enum class OutputMode { Info, Error, Warning, Debug };

    void Output(const std::string& id, const std::string& message, OutputMode mode) const;
    void Flush() const;
};