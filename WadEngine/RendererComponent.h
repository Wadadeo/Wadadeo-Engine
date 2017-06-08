#pragma once

#include "GL/glew.h"
#include <GL/gl.h>

#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Camera.h"

class GraphicsSystem;

class RendererComponent : public Component
{
public:
	static GraphicsSystem*	system;
	static GLenum			globalDrawMode;

private:
	MeshComponent *			_meshComponent;
	TransformComponent *	_transformComponent;
	
	Shader*		_shader;
	Material*	_material;

	GLenum	_drawMode;

public:
	RendererComponent(GameObject *attachTo);
	~RendererComponent();
	void setMaterial(Material* material);
	void setDrawMode(const GLenum& drawMode);
	//void onMeshChange();
	void draw(const Camera& camera, Shader *shader);
	void draw(const Camera& camera);
	void receive(int message);

	Material* material();

private:
	void	enableLocalDrawMode();
	void	disableLocalDrawMode();

};

