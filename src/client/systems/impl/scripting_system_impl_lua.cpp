#include "scripting_system_impl_lua.h"

bool ScriptingSystemImplLua::InitImpl() {
	m_lua.open_libraries(sol::lib::base);

    return true;
}

void ScriptingSystemImplLua::ShutdownImpl() {
}