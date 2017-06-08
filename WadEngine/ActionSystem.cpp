#include "ActionSystem.h"
#include "ActionScript.h"


ActionSystem* ActionScript::system = 0;

ActionSystem::ActionSystem()
{
	ActionScript::system = this;
	lState = luaL_newstate();
	luaL_openlibs(lState);
}


ActionSystem::~ActionSystem()
{
	lua_close(lState);
}

void ActionSystem::init()
{



	for (auto action : _behaviors) action->init();
}

void ActionSystem::update(float deltaTime)
{
	for (auto action : _behaviors) action->update(deltaTime);
}

void ActionSystem::add(ActionScript * action)
{
	_behaviors.push_back(action);
}
