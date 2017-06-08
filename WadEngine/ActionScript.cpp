#include "ActionScript.h"
#include "ActionSystem.h"


ActionScript::ActionScript(GameObject *attachTo)
	: Component(attachTo, ACTION_SCRIPT)
{
}


ActionScript::~ActionScript()
{
}

void ActionScript::receive(int message)
{
}
