#pragma once

#include <systems/system.h>

#include <resources/script.h>

#include <resources/resource_info.h>

class ScriptingSystem : public ISystem
{
public:
    static ScriptingSystem& Instance();

protected:
    ScriptingSystem() = default;;
};