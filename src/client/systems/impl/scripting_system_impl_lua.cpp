#include "scripting_system_impl_lua.h"

bool ScriptingSystemImplLua::Init() {
    
	m_lua.open_libraries(sol::lib::base);

    return true;
}

void ScriptingSystemImplLua::Shutdown() {

}