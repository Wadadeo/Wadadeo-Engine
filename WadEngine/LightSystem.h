#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "Camera.h"
#include "Shader.h"

using namespace std;
using namespace glm;

struct LightIndex
{
	unsigned int pointIndex;
	unsigned int spotIndex;
	unsigned int directionalIndex;

	LightIndex() : pointIndex(0), spotIndex(0), directionalIndex(0) {}
};

class LightComponent;

class LightSystem
{
	vector<LightComponent*> _lights;

	//ambiant light infos
	vec3	_ambiantBaseColor;
	float	_ambiantIntensity;

	Shader* _debugLightShader;
	bool	_drawDebugLight;

public:
	LightSystem();
	LightSystem(const LightSystem& other);
	~LightSystem();

	void init(Shader* debugLightShader);
	void update(const Camera& camera);

	void addLight(LightComponent* light);
	void remove(LightComponent* light);

	void setAmbiantColor(const glm::vec3 &color);
	void setAmbiantIntensity(const float &intensity);
	void loadLightsInShader(Shader* shader) const;


	const vector<LightComponent*>& getLights() const;
	const vec3& ambiantBaseColor() const;
	const float& ambiantIntensity() const;
	vec3 ambiantColor() const;
};

