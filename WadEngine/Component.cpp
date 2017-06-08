#include "Component.h"



Component::Component(GameObject *attachedTo, const eComponentType t) 
	: gameObject(attachedTo), type(t)
{
}

Component::~Component(){}
