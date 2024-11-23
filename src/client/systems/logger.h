#pragma once

#include <systems/system_lifecycle_declare.h>

#include <string>
#include <util/type_name.h>
#include <sstream>

#define _LOGGER_FUNCTION(FuncName)\
template<typename T> static void FuncName(const std::string& message) { Instance().Output(type_name<T>(), message, OutputMode::FuncName); }\
template<typename T> static void FuncName(const T& id, const std::string& message) { FuncName(type_name<T>(), message); }\
template<typename T, typename... Args> static void FuncName(Args... args) { std::stringstream ss; ((ss << args), ...); FuncName<T>(ss.str()); }\
template<typename T, typename... Args> static void FuncName(const T& id, Args... args) { FuncName<T>(args...); }

#define _LOGGER_FUNCTION_EMPTY(FuncName)\
template<typename T> static void FuncName(const std::string& message) {}\
template<typename T> static void FuncName(const T& id, const std::string& message) {}\
template<typename T, typename... Args> static void FuncName(Args... args) {}\
template<typename T, typename... Args> static void FuncName(const T& id, Args... args) {}

class Logger final {
    FAERIE___SYSTEM_SINGLETON_LIFECYCLE_DECLARE(Logger)
public:
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
    enum class OutputMode { Info, Error, Warning, Debug };

    void Output(const std::string& id, const std::string& message, OutputMode mode) const;
    void Flush() const;
};