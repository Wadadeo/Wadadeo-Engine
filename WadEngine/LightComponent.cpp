#include "LightComponent.h"
#include "GameObject.h"
#include "glm/ext.hpp"
#include "Primitive.h"

LightComponent::LightComponent(GameObject *attachTo) : Component(attachTo, LIGHT_COMPONENT)
{
	_type = POINT_LIGHT;
	_color = glm::vec3(1, 1, 1);
	_intensity = 1;
	transform = gameObject->getComponent<TransformComponent>();
	initDebugMesh();
	system->addLight(this);
}


LightComponent::~LightComponent()
{
	system->remove(this);
}

void LightComponent::initDebugMesh()
{
	SimplifiedMesh debugCube = genSimpleCube(0.3f);
	elementCount = debugCube.indices.size();

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, debugCube.vertices.size() * sizeof(GLfloat), &(debugCube.vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(GLuint), &(debugCube.indices[0]), GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void LightComponent::drawDebugMesh(const Camera& camera, Shader * debugLightShader)
{
	//cout << "draw debug light" << endl;

	debugLightShader->use();
	debugLightShader->uniform(MODEL_MATRIX, gameObject->transform->modelMatrix());
	debugLightShader->uniform(VIEW_MATRIX, camera.getViewMatrix()); //converting to mat3 and then mat4 remove translations
	debugLightShader->uniform(PROJECTION_MATRIX, camera.getProjectionMaxtrix());
	debugLightShader->uniform("color", _color * _intensity);

	glBindVertexArray(_VAO);
	glDrawElements(
		GL_TRIANGLES,		// mode
		elementCount,		// count
		GL_UNSIGNED_INT,	// type
		0					// element array buffer offset
	);
	glBindVertexArray(0);
}

void LightComponent::sendLightToShader(Shader *shader, LightIndex *index)
{
	glm::vec3 finalColor = _color * _intensity;
	std::string location;
	switch (_type) {
		case POINT_LIGHT:
			location = "pointLights[" + std::to_string(index->pointIndex) + "].";
			shader->uniform((location + "position").c_str(), transform->position());
			shader->uniform((location + "constant").c_str(), 1.0f);
			shader->uniform((location + "linear").c_str(), _linear);
			shader->uniform((location + "quadratic").c_str(), _quadratic);
			shader->uniform((location + "intensity").c_str(), finalColor);
			index->pointIndex += 1;
			break;
		case SPOT_LIGHT:
			location = "spotLights[" + std::to_string(index->spotIndex) + "].";

			shader->uniform((location + "position").c_str(), transform->position());
			shader->uniform((location + "direction").c_str(), getLightDirection());
			shader->uniform((location + "cutOff").c_str(), glm::cos(glm::radians(_angle)));
			shader->uniform((location + "outerCutOff").c_str(), glm::cos(glm::radians(_angle + _cutOff)));
			shader->uniform((location + "intensity").c_str(), finalColor);
			index->spotIndex += 1;
			break;
		case DIRECTIONAL_LIGHT:		
			location = "directionalLights[" + std::to_string(index->directionalIndex) + "].";
			shader->uniform((location + "direction").c_str(), getLightDirection());
			shader->uniform((location + "intensity").c_str(), finalColor);
			index->directionalIndex += 1;
			break;
	}
}

void LightComponent::setColor(const glm::vec3 & color)		{ _color = color; }
void LightComponent::setIntensity(const float & intensity)	{ _intensity = intensity; }
void LightComponent::setType(eLightType type)				{ _type = type;}


const eLightType & LightComponent::lightType() const { return _type; }

const glm::vec3 & LightComponent::getColor()		{ return _color;  }
const float & LightComponent::getIntensity()		{ return _intensity; }
const glm::vec3 & LightComponent::getLightPosition(){ return transform->position(); }

const glm::vec3 LightComponent::getLightDirection()
{
	// TODO: change to real method
	return -1 * getLightPosition(); //always look at the orign
}


void LightComponent::receive(int message)
{
	if (message == ON_TRANSFORM_CHANGE) {
		//do stuff
	}
}

