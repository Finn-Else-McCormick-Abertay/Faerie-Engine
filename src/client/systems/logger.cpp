#include "logger.h"
#include <systems/system_lifecycle_define.h>

#include <iostream>

FAERIE___SYSTEM_SINGLETON_INSTANCE_DEFINE_DEFAULT(Logger)
FAERIE___SYSTEM_SINGLETON_INIT_SHUTDOWN_DEFINE(Logger)

bool Logger::__Internal_Init() {
    Flush();
    return true;
}

void Logger::__Internal_Shutdown() {}

#define ANSI_RESET "\033[0m"
#define ANSI_RED "\033[31m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_MAGENTA "\033[35m"

void Logger::Output(const std::string& id, const std::string& message, OutputMode mode) const {
    std::string ansiStart, ansiEnd = ANSI_RESET;
    switch (mode) {
        case OutputMode::Error:     { ansiStart = ANSI_RED; }       break;
        case OutputMode::Warning:   { ansiStart = ANSI_YELLOW; }    break;
        case OutputMode::Debug:     { ansiStart = ANSI_MAGENTA; }   break;
        default: { ansiStart = ""; ansiEnd = ""; } break;
    }
    std::cout << ansiStart << "[" + id  + "] : " << message << ansiEnd << "\n";
}

void Logger::Flush() const {
    std::cout << std::endl;
}