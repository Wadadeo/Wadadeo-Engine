#pragma once

#include <vector>
#include "GL/glew.h"
#include <GL/gl.h>
#include "Mesh.hpp"

using namespace std;
using namespace glm;

enum ePrimitiveMesh
{	
	CUBE,
	SPHERE,
	TORUS,
	QUAD,
	EMPTY
};

Mesh generateCube();
Mesh generateSphere(const float& radius, const unsigned int& slices, const unsigned int& stacks);
Mesh generateTorus(const int &sides, const int &rings, const float &outerRadius, const float &innerRadius);
Mesh generateQuad(const unsigned int &size);

SimplifiedMesh genSimpleCube(float size);