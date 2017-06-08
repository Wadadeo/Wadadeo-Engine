#pragma once

#include <vector>
#include <iostream>

#include "GL/glew.h"
#include <GL/gl.h>

#include "Texture.hpp"
#include "Material.h"
#include "Vertex.hpp"

using namespace std;

class Mesh
{
	vector<Vertex>	_vertices;
	vector<GLuint>	_indices;
	Material*		_material;

public:
	const string	name;


	Mesh() {}
	Mesh(const char* name, const vector<Vertex> &vertices, const vector<GLuint> &indices, Material* material) :
		name(name), _vertices(vertices), _indices(indices), _material(material)
	{}

	~Mesh() {}




	const vector<Vertex>& vertices() { return _vertices; }
	const vector<GLuint>& indices() { return _indices; }
	Material* material() { return _material; }
};

//	Mesh with only vertex position and indices
// texcoords, normals, tangent & bitangent are not represented 
// MAINLY USED FOR DEBUG (light position, ...)
struct SimplifiedMesh
{
	vector<GLfloat> vertices;
	vector<GLuint> indices;
};

/*
void calcTangentAndBitTangent(Vertex & v1, Vertex & v2, Vertex & v3)
{
	glm::vec3 tangent, bitangent;

	// - triangle 1
	glm::vec3 edge1 = v2.position - v1.position;
	glm::vec3 edge2 = v3.position - v1.position;
	glm::vec2 deltaUV1 = v2.texCoords - v1.texCoords;
	glm::vec2 deltaUV2 = v3.texCoords - v1.texCoords;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);
}*/