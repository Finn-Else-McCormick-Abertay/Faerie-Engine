#pragma once

#include <systems/scripting_system.h>

#include <sol/sol.hpp>

class ScriptingSystemImplLua : public IScriptingSystem
{
public:

private:
    virtual bool InitImpl() override;
    virtual void ShutdownImpl() override;

    sol::state m_lua;
};