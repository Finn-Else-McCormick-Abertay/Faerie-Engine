#pragma once

#include <systems/system.h>

class ScriptingSystem : public ISystem
{
public:
    static ScriptingSystem& Instance();

protected:
    ScriptingSystem() = default;
};