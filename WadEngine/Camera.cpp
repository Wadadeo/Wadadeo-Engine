#include "Camera.h"
#include "InputSystem.h"


Camera::Camera()
{
	m_position = vec3(5, 5, 5);
	frontDirection = vec3(-1.0f, -1.0f, -1.0f);
	//targetPosition = vec3(0, 0, 0);
	fov = 45;
	upDirection = vec3(0, 1, 0);
	speed = 5.0f;
	sensitivity = 0.05f;
}


Camera::~Camera()
{
}

void Camera::useSceenDimensions(int screenWidth, int screenHeight)
{
	aspectRatio = (float)screenWidth / (float)screenHeight;
}

void Camera::update(float deltaTime)
{
	float cameraSpeed = speed * deltaTime;

	if (InputSystem::keyPressed(GLFW_KEY_W))
		m_position += cameraSpeed * frontDirection;
	if (InputSystem::keyPressed(GLFW_KEY_S))
		m_position -= cameraSpeed * frontDirection;
	if (InputSystem::keyPressed(GLFW_KEY_A))
		m_position -= glm::normalize(glm::cross(frontDirection, upDirection)) * cameraSpeed;
	if (InputSystem::keyPressed(GLFW_KEY_D))
		m_position += glm::normalize(glm::cross(frontDirection, upDirection)) * cameraSpeed;


	if (!InputSystem::mouseCliked(GLFW_MOUSE_BUTTON_2)) return;

	vec2 offset = InputSystem::mouseOffsetSinceLastFrame();
	//if (offset.x == 0 && offset.y == 0) return;

	offset *= sensitivity;


	yaw += offset.x;
	pitch += offset.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontDirection = glm::normalize(front);

}

mat4 Camera::getViewMatrix() const
{
	return lookAt(m_position, m_position + frontDirection, upDirection);
}

mat4 Camera::getProjectionMaxtrix() const
{
	return perspective(fov, aspectRatio, 0.1f, 500.0f);
}

vec3 Camera::position() const
{
	return m_position;
}
