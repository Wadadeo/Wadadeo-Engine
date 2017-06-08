#pragma once

#include "MeshComponent.h"
#include "Camera.h"
#include "Cubemap.hpp"

class SkyboxRenderer
{
public:
	bool		enabled = false;
private:
	static GLfloat cubeMapVertices[108];

	Shader*		_shader;
	CubeMap*	_cubemap;
	GLuint		_VAO;
	GLuint		_VBO;

public:
	SkyboxRenderer();
	SkyboxRenderer(CubeMap *cubemap, Shader* skyboxShader);
	SkyboxRenderer(const SkyboxRenderer& other);
	~SkyboxRenderer();

	void draw(Camera &camera);
	const CubeMap* cubeMap() const;

};

