#pragma once

#include "glm/glm.hpp"
#include "TransformComponent.h"
#include "LightSystem.h"

enum eLightType
{
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT
};

class LightComponent : public Component
{
	
	TransformComponent* transform;
	vec3		_color;
	float		_intensity;
	eLightType	_type;
	
	//point/spot Light specific
	float		distance;

	//spot Light specific
	float		_angle = 12.5; //in degree
	float		_cutOff = 5;

	//attenuation
	float		_linear = 0.7f;
	float		_quadratic = 0.17f;


	/* DEBUG */

	GLuint _VAO;
	GLuint _VBO;
	GLuint _EBO;
	GLuint elementCount;

public:
	static LightSystem* system;


	LightComponent(GameObject *attachTo);
	~LightComponent();



	//fill light uniform in shader 
	void sendLightToShader(Shader *shader, LightIndex *index);

	//setters
	void setColor(const glm::vec3& color);
	void setIntensity(const float& intensity);
	void setType(eLightType type);

	//getters
	const eLightType& lightType() const;
	const glm::vec3& getColor();
	const float& getIntensity();
	const glm::vec3& getLightPosition();
	const glm::vec3 getLightDirection();

	void receive(int message);

	/* DEBUG */

	//create a simplified mesh in the scene at the light position for debug purpose
	void initDebugMesh();
	//draw the debug mesh
	void drawDebugMesh(const Camera& camera, Shader *debugLightShader);

};

