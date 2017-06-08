#pragma once

#include <string>
#include <iostream>
#include <list>
#include <typeinfo> 

#include "Component.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "LightComponent.h"
#include "RendererComponent.h"

#include "Model.hpp"

class GameObject
{
private:
	RendererComponent*		_renderer;
	std::list<Component *>	_components;

public:
	TransformComponent*		transform;
	std::string	name;
	bool selected;

	GameObject(const std::string &name);
	GameObject(Model* model);
	~GameObject();

	//components
	template<class T> 
	T * addComponent() 
	{
		T * cmp = new T(this);
		_components.push_back(cmp);
		return cmp;
	};

	template<class T> 
	T * getComponent()
	{
		for (auto it = _components.begin(); it != _components.end(); it++) {
			T *result = dynamic_cast<T*>(*it);
			if (result)
				return result;
		}
		return NULL;
	};
	
	void send(int message);
	void setRenderer(RendererComponent* renderer);
	void draw();
};

