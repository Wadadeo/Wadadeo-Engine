#pragma once

#include "GameObject.h"

#include "AssetCatalogue.h"

class MainWindow;

class Scene
{
	AssetCatalogue*			_assets;
	vector<TransformComponent*>	_hierarchy;

public:
	Scene(AssetCatalogue* assets);
	~Scene();

	const vector<TransformComponent*>& hierarchy();
	void load();
	
	//to Move in ObjectFactory
	void addGameobjectOfModel(Model* model);
	void removeFromScene(GameObject *obj);
	GameObject* addGameobjectOfPrimitive(ePrimitiveMesh type, const string& name);
	GameObject* addLight(eLightType type, const string& name);
};

