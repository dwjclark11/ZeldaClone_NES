#pragma once
#include <sol/sol.hpp>

class ScriptComponent
{
public:
	sol::function func;

	ScriptComponent(sol::function func = sol::lua_nil)
	{
		this->func = func;
	}
};