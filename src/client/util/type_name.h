#pragma once

#include <string>
#include <typeinfo>
#include <cxxabi.h>

inline std::string demangle(const char* mangledName) {
    int status = 0;
    char* demangledName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
    std::string returnString;
    if (status == 0) { returnString.assign(demangledName); } else { returnString.assign(mangledName); }
    free(demangledName);
    return returnString;
}

template<typename T> std::string type_name() {
    return demangle(typeid(T).name());
}