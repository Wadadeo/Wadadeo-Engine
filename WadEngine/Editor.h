#pragma once
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include "Scene.h"
#include "GraphicsSystem.h"
#include "AssetCatalogue.h"
#include "InspectorWindow.h"


#define IMPORT_MODEL 1
#define IMPORT_TEXTURE 2

class Editor
{

	InspectorWindow _inspector;

	AssetCatalogue* _assetCatalogue;


	//GameObject *selected;
	bool displayWindows;
	bool *shouldExit;

	bool showImportWindow;

	short importState = 0;

public:
	Editor();
	~Editor();

	bool init(GLFWwindow* window, bool *shouldExit, AssetCatalogue *assetCatalogue, Scene *scene);
	void update(AssetCatalogue* assets, GraphicsSystem *graphics, Scene *scene);


	//MENU
	void displayMainMenu(Scene *scene);


	//HIERARCHY OF GAMEOBJECT
	void hierarchyPanel(Scene *scene);
	void displayTransformNode(const vector<TransformComponent*>& transforms);
	void onSelection(TransformComponent* tranform);

	//CAMERA OPTIONS
	void cameraPanel(GraphicsSystem* graphics);

	void bloomPanel(BloomEffect &bloom);

	void importPanel(AssetCatalogue* assets, Scene *scene);

	/*void displaySelectedGameObject(Scene *scene, AssetCatalogue* assets);
	void displayTransform(TransformComponent* transform);
	void displayRenderer(RendererComponent* renderer, AssetCatalogue* assets);
	void displayMesh(MeshComponent* meshCpt);
	void displayLight(LightComponent* lightCpt);*/


	/* ASSET PANEL */
	void assetPanel(AssetCatalogue* assets);


};

