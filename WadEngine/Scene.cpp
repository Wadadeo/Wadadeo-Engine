#include "Scene.h"
#include "RotateScript.h"

Scene::Scene(AssetCatalogue* assets) : _assets(assets)
{}

Scene::~Scene() 
{}

const vector<TransformComponent*>& Scene::hierarchy()
{
	return _hierarchy;
}


void Scene::load()
{
	//Tron character
	Model* tronModel = _assets->addModel(MESH_FOLDER("Light Cycle/HQ_Movie cycle.obj"));
	GameObject* tron = new GameObject(tronModel);
	tron->transform->Rotate({ -90, 0, 0 });
	_hierarchy.push_back(tron->transform);

	//floor
	Material* wadMat = _assets->addMaterial("WadTexture");
	wadMat->setMainTexture(_assets->addTexture("wad", TEXTURE_FOLDER("wad_diffuse.png")));
	wadMat->setGlowMap(_assets->addTexture("wad_glow", TEXTURE_FOLDER("wad_glow.jpg"))->id);
	wadMat->setSpecular({0.4, 0.4, 0.4});

	GameObject *floor = new GameObject("floor");
	floor->addComponent<MeshComponent>()->mesh(QUAD);
	floor->addComponent<RendererComponent>()->setMaterial(wadMat);
	_hierarchy.push_back(floor->transform);

	//lights
	GameObject *l1 = new GameObject("green Light");
	l1->transform->Translate({ 0, 1, 3 });
	LightComponent* lightCpt = l1->addComponent<LightComponent>();
	lightCpt->setType(POINT_LIGHT);
	lightCpt->setColor({ 0, 1, 0 });
	lightCpt->setIntensity(5);
	_hierarchy.push_back(l1->transform);

	GameObject *l2 = new GameObject("blue Light");
	l2->transform->Translate({ -1, 2, -3 });
	lightCpt = l2->addComponent<LightComponent>();
	lightCpt->setType(POINT_LIGHT);
	lightCpt->setColor({ 0.05, 0.2, 1});
	lightCpt->setIntensity(7);
	_hierarchy.push_back(l2->transform);

	GameObject *l3 = new GameObject("white Light");
	l3->transform->Translate({ 4, 1, 1 });
	lightCpt = l3->addComponent<LightComponent>();
	lightCpt->setType(POINT_LIGHT);
	lightCpt->setColor({ 1, 1, 1 });
	lightCpt->setIntensity(4);
	_hierarchy.push_back(l3->transform);


}

void Scene::addGameobjectOfModel(Model * model)
{
	GameObject *obj = new GameObject(model);
	//obj->addComponent<RotateScript>();
	_hierarchy.push_back(obj->transform);
}

void Scene::removeFromScene(GameObject * obj)
{
	for (auto it = _hierarchy.begin(); it != _hierarchy.end(); ++it)
	{
		if (*it == obj->transform)
		{
			_hierarchy.erase(it);
			break;
		}
	}
	delete obj;
}

GameObject* Scene::addGameobjectOfPrimitive(ePrimitiveMesh type, const string& name)
{
	GameObject *primitive = new GameObject(name);
	primitive->addComponent<MeshComponent>()->mesh(type);
	primitive->addComponent<RendererComponent>()->setMaterial(_assets->defaultMaterial());
	_hierarchy.push_back(primitive->transform);
	return primitive;
}

GameObject * Scene::addLight(eLightType type, const string & name)
{
	GameObject *light = new GameObject(name);
	light->addComponent<LightComponent>()->setType(type);
	_hierarchy.push_back(light->transform);
	return light;
}



