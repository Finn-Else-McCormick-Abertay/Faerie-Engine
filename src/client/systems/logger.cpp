#include "logger.h"

#include <iostream>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

bool Logger::InitImpl() {
    Flush();
    Info(*this, "Initialised");
    return true;
}

void Logger::ShutdownImpl() {}

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