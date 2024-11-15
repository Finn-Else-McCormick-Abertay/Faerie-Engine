#pragma once

#include <systems/system.h>

#include <string>
#include <sstream>

#define _LOGGER_FUNCTION(FuncName)\
static void FuncName (const std::string& id, const std::string& message) { Instance().Output(id, message, OutputMode::FuncName); }\
template<typename T> static void FuncName (const T& id, const std::string& message) { std::stringstream ss; ss << id; FuncName (ss.str(), message); }\
template<typename... Args> static void FuncName (const std::string& id, Args... args) { std::stringstream ss; ((ss << args), ...); FuncName (id, ss.str()); }\
template<typename T, typename... Args> static void FuncName (const T& id, Args... args) { std::stringstream ss; ((ss << args), ...); FuncName (id, ss.str()); }

#define _LOGGER_FUNCTION_EMPTY(FuncName)\
static void FuncName (const std::string& id, const std::string& message) {}\
template<typename T> static void FuncName (const T& id, const std::string& message) {}\
template<typename... Args> static void FuncName (const std::string& id, Args... args) {}\
template<typename T, typename... Args> static void FuncName (const T& id, Args... args) {}

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