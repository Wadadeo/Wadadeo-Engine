#include "MeshComponent.h"
#include "GameObject.h"

Mesh MeshComponent::primitives[4] = {
	generateCube(),
	generateSphere(1, 64, 64),
	generateTorus(64, 64, 1, 0.5),
	generateQuad(5)
};

MeshComponent::MeshComponent(GameObject *attachTo, ePrimitiveMesh primitive) :
	Component(attachTo, MESH_COMPONENT), _type(primitive) 
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
}

MeshComponent::~MeshComponent(){}

void MeshComponent::mesh(Mesh *mesh)
{
	if (!mesh) return;
	_mesh = mesh;
	//assert(mesh->vertices().size() > 0);
	assert(mesh->indices().size() > 0);

	const Vertex *firstVertex = &(_mesh->vertices()[0]);
	const GLuint *firstIndex = &(_mesh->indices()[0]);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices().size() * sizeof(Vertex), firstVertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices().size() * sizeof(GLuint), firstIndex, GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, texCoords));
	//Vertex Tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, tangent));

	//Vertex Tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, bitTangent));

	glBindVertexArray(0);

	if (gameObject) gameObject->send(ON_MESH_CHANGE);
}

void MeshComponent::mesh(const ePrimitiveMesh & primitive)
{
	_type = primitive;
	mesh(&(primitives[primitive]));
}

void MeshComponent::receive(int message)
{
}

Mesh * MeshComponent::mesh()
{
	return _mesh;
}

const GLuint & MeshComponent::VAO() { return _VAO; }
const GLsizei MeshComponent::elementCount() { return _mesh->indices().size(); }
const ePrimitiveMesh & MeshComponent::getType() { return _type; }


