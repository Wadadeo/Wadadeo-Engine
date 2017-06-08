#pragma once

// GLEW
#include <GL/glew.h>
#include <GL/gl.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Editor.h"
#include "AssetCatalogue.h"
#include "InputSystem.h"
#include "ActionSystem.h"
#include "GraphicsSystem.h"

#include "Scene.h"

#define		WINDOW_DEFAULT_WIDTH	1360
#define		WINDOW_DEFAULT_HEIGHT	768

class Engine
{
	int width;
	int height;
	GLFWwindow* window;

	float lastFrame;
	float deltaTime;
	bool shouldExit;

	Editor editor;

	//engine systems
	InputSystem		inputs;
	ActionSystem	actions;
	GraphicsSystem	graphics;


	AssetCatalogue assetsCatalogue;
	Scene* scene;


public:
	Engine();
	~Engine();

	bool init();
	void mainLoop();
};

