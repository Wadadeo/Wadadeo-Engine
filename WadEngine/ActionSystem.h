#pragma once

#include <lua.hpp>
#include "ActionScript.h"

class ActionSystem
{
	vector<ActionScript*> _behaviors;

	lua_State *lState;

public:
	ActionSystem();
	~ActionSystem();

	void init();
	void update(float deltaTime);

	void add(ActionScript* action);
};

