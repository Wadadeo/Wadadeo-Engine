#include "TransformComponent.h"
#include "GameObject.h"
#include "glm\ext.hpp"
#include "math.h"

TransformComponent::TransformComponent(GameObject *attachTo) :
	Component(attachTo, TRANSFORM_COMPONENT)
{
	Translate(glm::vec3(0, 0, 0));
	Rotate(glm::vec3(0, 0, 0));
	Scale(glm::vec3(1, 1, 1));
}


TransformComponent::TransformComponent(const TransformComponent & other) : 
	Component(other.gameObject, TRANSFORM_COMPONENT)
{
	_parent = other._parent;
	childs = other.childs;
	_translationMatrix = other._translationMatrix;
	_rotationMatrix = other._rotationMatrix;
	_scaleMatrix = other._scaleMatrix;

	_worldPosition = other._worldPosition;
	_worldRotation = other._worldRotation;
	_worldScale = other._worldScale;

	_localPosition = other._localPosition;
	_localRotation = other._localRotation;
	_localScale = other._localScale;
	_rotation = other._rotation;
}

TransformComponent::~TransformComponent() 
{
	for (auto child : childs)
		delete child->gameObject;
}


void TransformComponent::Translate(const vec3 &position)
{
	_localPosition = position;

	if (!_parent)	_worldPosition = _localPosition;
	else			_worldPosition = _parent->position() + _localPosition;
	
	_translationMatrix = glm::translate(glm::mat4(1.0f), _worldPosition);
	actualizeModelMatrix();

	for (auto it = childs.begin(); it != childs.end(); it++)
		(*it)->Translate(position);

	//if (_componentUI) _componentUI->onPositionChange(_localPosition);
	gameObject->send(ON_TRANSFORM_CHANGE);
}

void TransformComponent::Rotate(const vec3 &rotation)
{
	_localRotation.x = fmod(rotation.x, 360.0f);
	_localRotation.y = fmod(rotation.y, 360.0f);
	_localRotation.z = fmod(rotation.z, 360.0f);

	if (!_parent)	_worldRotation = _localRotation;
	else			_worldRotation = _parent->rotation() /*+ _localRotation*/;

	_rotation = glm::quat(glm::radians(_worldRotation));
	_rotationMatrix = glm::toMat4(_rotation);
	actualizeModelMatrix();

	for (auto it = childs.begin(); it != childs.end(); it++)
		(*it)->Rotate(rotation);
	//if (_componentUI) _componentUI->onRotationChange(_localRotation);
	gameObject->send(ON_TRANSFORM_CHANGE);
}

void TransformComponent::Scale(const vec3 &scale)
{
	_localScale = scale;

	if (!_parent)	_worldScale = _localScale;
	else			_worldScale = _parent->scale() + _localScale;

	_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_worldScale.x, _worldScale.y, _worldScale.z));
	actualizeModelMatrix();

	for (auto it = childs.begin(); it != childs.end(); it++)
		(*it)->Scale(scale);
	//if (_componentUI) _componentUI->onScaleChange(_localScale);
	gameObject->send(ON_TRANSFORM_CHANGE);
}

void TransformComponent::receive(int message)
{
}



void TransformComponent::actualizeModelMatrix()
{
	_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
}

void TransformComponent::removeChild(TransformComponent * child)
{
	for (auto it = childs.begin(); it != childs.end(); it++)
		if (*it == child)
			childs.erase(it);
}

void TransformComponent::addChild(TransformComponent *newChild)
{
	childs.push_back(newChild);
}


const mat4& TransformComponent::modelMatrix() { return _modelMatrix; }
const vec3&	TransformComponent::position()	{ return _localPosition; }
const vec3&	TransformComponent::scale()		{ return _localScale; }
const vec3&	TransformComponent::rotation()	{ return _localRotation; }
TransformComponent * TransformComponent::parent()	{ return _parent; }
const vector<TransformComponent*>& TransformComponent::getChilds() { return childs; }


void TransformComponent::setParent(TransformComponent * newParent)
{
	if (_parent)
		_parent->removeChild(this);
	_parent = newParent;
	if (newParent)
		newParent->addChild(this);
}



