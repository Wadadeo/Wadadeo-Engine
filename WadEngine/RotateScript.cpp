#include "RotateScript.h"
#include "ActionSystem.h"

RotateScript::RotateScript(GameObject *attachTo) : ActionScript(attachTo)
{
	system->add(this);
	init();
}
RotateScript::~RotateScript() {}

void RotateScript::init()
{
	transform = gameObject->transform;
	rotateSpeed = 15.0f;
}

void RotateScript::update(float deltaTime)
{
	vec3 orientation = transform->rotation();
	orientation.y += rotateSpeed * deltaTime;

	transform->Rotate(orientation);
}
