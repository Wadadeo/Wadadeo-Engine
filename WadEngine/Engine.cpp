#include <iostream>
#include "Engine.h"

using namespace std;

Engine::Engine() : width(WINDOW_DEFAULT_WIDTH), height(WINDOW_DEFAULT_HEIGHT)
{
	shouldExit = false;
}


Engine::~Engine()
{
	glfwTerminate();
}

bool Engine::init()
{
	if (!glfwInit())
		exit(1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Wadadeo Engine v1", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	inputs.init(window);

	assetsCatalogue.initEngineAssets();
	scene = new Scene(&assetsCatalogue);
	scene->load();


	editor.init(window, &shouldExit, &assetsCatalogue, scene);

	graphics.init(width, height, &assetsCatalogue);




	actions.init();

	return true;
}

void Engine::mainLoop()
{
	// Main loop
	while (!shouldExit)
	{
		//calc delta time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		inputs.update();

		//glfwGetFramebufferSize(window, &display_w, &display_h);

		actions.update(deltaTime);

		editor.update(&assetsCatalogue, &graphics, scene);

		if (InputSystem::keyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window))
			shouldExit = true;


		// Rendering
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		graphics.update(deltaTime);

		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}
