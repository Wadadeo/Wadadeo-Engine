#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component.h"

using namespace glm;

class TransformComponent : public Component
{	
	TransformComponent* _parent = nullptr;
	std::vector<TransformComponent*> childs;

	mat4 _modelMatrix;
	mat4 _translationMatrix;
	mat4 _rotationMatrix;
	mat4 _scaleMatrix;

	vec3 _worldPosition;
	vec3 _worldRotation;
	vec3 _worldScale;

	vec3 _localPosition;
	vec3 _localRotation;
	vec3 _localScale;

	quat _rotation;

public:
	TransformComponent(GameObject *attachTo);
	TransformComponent(const TransformComponent& other);
	~TransformComponent();

	void Translate(const vec3 &position);
	void Rotate(const vec3 &rotation);
	void Scale(const vec3 &scale);
	void receive(int message);


private:
	void actualizeModelMatrix();
	void removeChild(TransformComponent *child);
	void addChild(TransformComponent *newChild);



public:
	const mat4& modelMatrix();
	const vec3& position();
	const vec3& scale();
	const vec3& rotation();
	TransformComponent* parent();
	const std::vector<TransformComponent*>& getChilds();

	void setParent(TransformComponent *parent);

};

