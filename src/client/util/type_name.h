#pragma once

#include <string>
#include <typeinfo>
#include <cxxabi.h>

template<typename T> std::string type_name() {
    const char* mangledName = typeid(T).name();
    int status = 0;
    char* demangledName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
    std::string returnString;
    if (status == 0) { returnString.assign(demangledName); } else { returnString.assign(mangledName); }
    free(demangledName);
    return returnString;
}