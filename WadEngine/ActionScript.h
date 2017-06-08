#pragma once
#include "Component.h"
#include "GameObject.h"

class ActionSystem;

class ActionScript : public Component
{
public:

	static ActionSystem* system;

	ActionScript(GameObject *attachTo);
	virtual ~ActionScript();

	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;


	// Inherited via Component
	void receive(int message) override;

};

