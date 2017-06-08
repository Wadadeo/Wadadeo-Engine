#pragma once

// GLFW
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw_gl3.h"
#include "glm\glm.hpp"

using namespace::glm;

class InputSystem
{
public:
	
	static bool keys[1024];
	static bool mouseButtons[3];
	static double cursorX;
	static double cursorY;
	static double lastCursorX;
	static double lastCursorY;
	static double scrollY;

	InputSystem();
	~InputSystem();

	void init(GLFWwindow* window);
	void update();
	static bool keyPressed(int key);
	static bool mouseCliked(int button);
	static vec2 mouseOffsetSinceLastFrame();
	static vec2 mousePosition();
	static float scrollWheel();

};

//GLFW CALLBACKS
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void charCallback(GLFWwindow* window, unsigned int c);