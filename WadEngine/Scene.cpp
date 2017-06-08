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
	GameObject *l1 = new GameObject("Light 1");
	l1->transform->Translate(glm::vec3(5, 10, 3));
	l1->addComponent<LightComponent>()->setType(DIRECTIONAL_LIGHT);

	Material* test = _assets->addMaterial("textureTest");
	test->setMainTexture(_assets->addTexture("brick", TEXTURE_FOLDER("154.jpg")));
	test->setNormalTexture(_assets->addTexture("brick_norm", TEXTURE_FOLDER("154_norm.jpg")));

	Material* test2 = _assets->addMaterial("textureTest2");
	test2->setMainTexture(_assets->addTexture("brick_2", TEXTURE_FOLDER("brickwall.jpg")));
	test2->setNormalTexture(_assets->addTexture("brick_norm_2", TEXTURE_FOLDER("brickwall_normal.jpg")));


	Material* test3 = _assets->addMaterial("WadTexture");
	test3->setMainTexture(_assets->addTexture("wad", TEXTURE_FOLDER("wad_text.png")));

	/*Model* ogreModel = _assets->addModel("Assets/Meshes/chelou/bs_ears.obj");

	Material* ogreMaterial = _assets->addMaterial("ogre");
	ogreMaterial->setMainTexture(_assets->addTexture("diffuseOgre", "Assets/Meshes/chelou/ogre_diffuse.png"));
	ogreMaterial->setNormalTexture(_assets->addTexture("normalOgre", "Assets/Meshes/chelou/ogre_normalmap.png"));

	GameObject *ogreHead = new GameObject(ogreModel);
	GameObject *child = ogreHead->transform->getChilds()[0]->gameObject;
	child->getComponent<RendererComponent>()->setMaterial(ogreMaterial);*/

	/*GameObject *floor = new GameObject("Floor");
	floor->addComponent<MeshComponent>()->mesh(QUAD);
	floor->addComponent<RendererComponent>()->setMaterial(_assets->defaultMaterial());*/
	//floor->transform->Scale(vec3(3, 3, 3));

	//_hierarchy.push_back(ogreHead);
	_hierarchy.push_back(l1->transform);
	//_hierarchy.push_back(ogreHead->transform);
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



