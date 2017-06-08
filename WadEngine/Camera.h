#pragma once
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

using namespace glm;

class Camera
{
	vec3 m_position;
	vec3 frontDirection;
	vec3 upDirection;

	float fov;
	float speed;
	float sensitivity;
	float aspectRatio;

	float yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
	float pitch = 0.0f;

public:
	Camera();
	~Camera();

	void useSceenDimensions(int screenWidth, int screenHeight);

	void update(float deltaTime);

	mat4 getViewMatrix() const;
	mat4 getProjectionMaxtrix() const;
	vec3 position() const;
};

