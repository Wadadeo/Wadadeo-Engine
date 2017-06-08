#include "glm/gtx/string_cast.hpp"

#include "GameObject.h"
#include "RendererComponent.h"
#include "LightComponent.h"
#include "GraphicsSystem.h"

GLenum RendererComponent::globalDrawMode = GL_FILL;

RendererComponent::RendererComponent(GameObject *attachTo) : 
	Component(attachTo, RENDERER_COMPONENT)
{
	_drawMode = globalDrawMode;
	//_material = MaterialCatalogue::getInstance().getDefault();
	//_shader = _material->getShader();

	_transformComponent = gameObject->getComponent<TransformComponent>();
	_meshComponent = gameObject->getComponent<MeshComponent>();

	gameObject->setRenderer(this);
	system->add(this);
}


RendererComponent::~RendererComponent()
{
	system->remove(this);
}

void RendererComponent::setMaterial(Material * material)
{
	_material = material;
	_shader = _material->getShader();
}


void RendererComponent::draw(const Camera& camera, Shader * shader)
{
	shader->use();

	const glm::mat4 &model = _transformComponent->modelMatrix();
	shader->uniform(MODEL_MATRIX, model);
	shader->uniform(VIEW_MATRIX, camera.getViewMatrix());
	shader->uniform(PROJECTION_MATRIX, camera.getProjectionMaxtrix());
	shader->uniform(NORMAL_MATRIX, glm::mat3(glm::transpose(glm::inverse(model))));
	shader->uniform("viewPos", camera.position()); 	//camera postion


	//skybox
	//shader->uniform("hasSkybox", system->drawSkybox);
	//shader->uniform("skybox", (GLuint)0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, system->skyboxRenderer().cubemapID());

	//lights
	system->lightSystem().loadLightsInShader(shader);
	_material->sendInfoToShader();

	//draw
	//enableLocalDrawMode();
	glBindVertexArray(_meshComponent->VAO());
	glDrawElements(
		GL_TRIANGLES,								// mode
		_meshComponent->elementCount(),			// count
		GL_UNSIGNED_INT,							// type
		0											// element array buffer offset
	);
	glBindVertexArray(0);
	//disableLocalDrawMode();

	//glBindTexture(GL_TEXTURE_2D, 0);
	shader->disable();

}

void RendererComponent::draw(const Camera& camera) { draw(camera, _shader); }

void RendererComponent::setDrawMode(const GLenum & drawMode)
{
	_drawMode = drawMode;
}
void RendererComponent::enableLocalDrawMode()
{
	if (_drawMode != globalDrawMode)
		glPolygonMode(GL_FRONT_AND_BACK, _drawMode);
}
void RendererComponent::disableLocalDrawMode()
{
	if (_drawMode != globalDrawMode)
		glPolygonMode(GL_FRONT_AND_BACK, globalDrawMode);
}

void RendererComponent::receive(int message)
{
	if (message == ON_MESH_CHANGE) {
		//std::cout << "renderer knows that mesh has changed" << std::endl;
	}
	else if (message == ON_TRANSFORM_CHANGE) {
		//std::cout << "renderer knows that transform has changed" << std::endl;
	}
}

Material * RendererComponent::material()
{
	return _material;
}
