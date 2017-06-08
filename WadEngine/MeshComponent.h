#pragma once

#include <vector>

#include "glm\glm.hpp"
#include "Component.h"
#include "Mesh.hpp"
#include "Primitive.h"

class MeshComponent : public Component
{
	Mesh*	_mesh;
	GLuint	_VAO;
	GLuint	_VBO;
	GLuint	_EBO;

	ePrimitiveMesh	_type;

public:

	static Mesh primitives[4];


	MeshComponent(GameObject *attachTo, ePrimitiveMesh primitive = EMPTY);
	~MeshComponent();

	void mesh(Mesh* mesh);
	void mesh(const ePrimitiveMesh &primitive);
	void receive(int message);

	Mesh* mesh();
	const GLuint& VAO();
	const GLsizei elementCount();
	const ePrimitiveMesh& getType();
};


