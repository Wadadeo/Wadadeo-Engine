#pragma once
#include <imgui.h>
#include "AssetCatalogue.h"
#include "GameObject.h"
#include "Scene.h"

#define BETWEEN_PROPERTIES_SPACING 10

enum eInspectorState
{
	SHOW_GAMEOBJECT,
	SHOW_MATERIAL,
	BLANK
};

class InspectorWindow
{
	AssetCatalogue * assets;
	Scene *scene;

	eInspectorState displayState;
	void *			data;

public:
	InspectorWindow();
	~InspectorWindow();
	void init(AssetCatalogue * assetCatalogue, Scene *scene);
	void draw();

	//change what is drawn by the inspector
	void display(eInspectorState state, void *data);

private:
	void drawGameObject(GameObject* selected);
	void displayTransform(TransformComponent* transform);
	void displayRenderer(RendererComponent* renderer);
	void displayMesh(MeshComponent* meshCpt);
	void displayLight(LightComponent* lightCpt);

	void drawMaterial(Material* material);
	void diffuseProp(Material *material);
	void normalProp(Material *material);


};

void showTexturePreview(GLuint textureID);