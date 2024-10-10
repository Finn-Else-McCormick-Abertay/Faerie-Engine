#pragma once

#include <systems/scripting_system.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class ScriptingSystemImplLua : public IScriptingSystem
{
public:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    sol::state m_lua;
};