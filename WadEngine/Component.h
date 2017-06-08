#pragma once

#include <iostream>
#include <vector>

#define ON_MESH_CHANGE 1
#define ON_TRANSFORM_CHANGE 2

using namespace std;

class GameObject;

enum eComponentType {
	MESH_COMPONENT,
	TRANSFORM_COMPONENT,
	RENDERER_COMPONENT,
	LIGHT_COMPONENT,
	ACTION_SCRIPT,
};

class Component
{
public:

	GameObject *gameObject;	
	const eComponentType type;

	Component(GameObject *attachedTo, const eComponentType t);
	virtual ~Component();
	virtual void receive(int message) = 0;
};

