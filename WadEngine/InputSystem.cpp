#include "InputSystem.h"

bool InputSystem::keys[1024];
bool InputSystem::mouseButtons[3];
double InputSystem::cursorX = 0;
double InputSystem::cursorY = 0;
double InputSystem::lastCursorX = 0;
double InputSystem::lastCursorY = 0;
double InputSystem::scrollY;

InputSystem::InputSystem()
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCharCallback(window, charCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
}

void InputSystem::update()
{
	lastCursorX = cursorX;
	lastCursorY = cursorY;
	glfwPollEvents();
}

bool InputSystem::keyPressed(int key)
{
	return keys[key];
}

bool InputSystem::mouseCliked(int button)
{
	if (button >= 0 && button < 3)
		return mouseButtons[button];
	return false;
}

vec2 InputSystem::mouseOffsetSinceLastFrame()
{
	return vec2(cursorX - lastCursorX, lastCursorY - cursorY); // Reversed since y-coordinates go from bottom to left
}

vec2 InputSystem::mousePosition()
{
	return vec2(cursorX, cursorY);
}

float InputSystem::scrollWheel()
{
	return (float)scrollY;
}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods); //call UI implementation
	if (action == GLFW_PRESS)
		InputSystem::keys[key] = true;
	else if (action == GLFW_RELEASE)
		InputSystem::keys[key] = false;
}

void scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset); //call UI implementation
	InputSystem::scrollY = yoffset;
}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods); //call UI implementation
	if (action == GLFW_PRESS && button >= 0 && button < 3)
		InputSystem::mouseButtons[button] = true;
	if (action == GLFW_RELEASE && button >= 0 && button < 3)
		InputSystem::mouseButtons[button] = false;
}

void charCallback(GLFWwindow *window, unsigned int c)
{
	ImGui_ImplGlfwGL3_CharCallback(window, c); //call UI implementation
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool first = true;
	
	if (first) {
		InputSystem::lastCursorX = xpos;
		InputSystem::lastCursorY = ypos;
		first = false;
	}

	InputSystem::cursorX = xpos;
	InputSystem::cursorY = ypos;
}
