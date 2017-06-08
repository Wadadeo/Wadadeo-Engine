#include <glm/gtc/constants.hpp>
#include "Primitive.h"

Mesh generateCube()
{
	vector<Vertex> vertices = {
		//vec3 POSITION				vec3 NORMAL				vec2 TEXCOORD		vec3 Tangeant			vec3 BitTangent

		//front
		{ vec3(-1.0, -1.0, 1.0),	vec3(0.0, 0.0, 1.0),	vec2(0.0f, 1.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, -1.0, 0.0) },
		{ vec3(1.0, -1.0, 1.0),		vec3(0.0, 0.0, 1.0),	vec2(1.0f, 1.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, -1.0, 0.0) },
		{ vec3(1.0, 1.0, 1.0),		vec3(0.0, 0.0, 1.0),	vec2(1.0f, 0.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, -1.0, 0.0) },
		{ vec3(-1.0, 1.0, 1.0),		vec3(0.0, 0.0, 1.0),	vec2(0.0f, 0.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, -1.0, 0.0) },

		//right
		{ vec3(1.0, 1.0, 1.0),		vec3(1.0, 0.0, 0.0),	vec2(0.0f, 0.0f),	vec3(0.0, 0.0, -1.0),	vec3(0.0, 1.0, 0.0) },
		{ vec3(1.0, 1.0, -1.0),		vec3(1.0, 0.0, 0.0),	vec2(1.0f, 0.0f),	vec3(0.0, 0.0, -1.0),	vec3(0.0, 1.0, 0.0) },
		{ vec3(1.0, -1.0, -1.0),	vec3(1.0, 0.0, 0.0),	vec2(1.0f, 1.0f),	vec3(0.0, 0.0, -1.0),	vec3(0.0, 1.0, 0.0) },
		{ vec3(1.0, -1.0, 1.0),		vec3(1.0, 0.0, 0.0),	vec2(0.0f, 1.0f),	vec3(0.0, 0.0, -1.0),	vec3(0.0, 1.0, 0.0) },

		//back
		{ vec3(-1.0, -1.0, -1.0),	vec3(0.0, 0.0, -1.0),	vec2(1.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 1.0, 0.0) },
		{ vec3(1.0, -1.0, -1.0),	vec3(0.0, 0.0, -1.0),	vec2(0.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(1.0, 1.0, -1.0),		vec3(0.0, 0.0, -1.0),	vec2(0.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, 1.0, -1.0),	vec3(0.0, 0.0, -1.0),	vec2(1.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },

		//left
		{ vec3(-1.0, -1.0, -1.0),	vec3(-1.0, 0.0, 0.0),	vec2(0.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, -1.0, 1.0),	vec3(-1.0, 0.0, 0.0),	vec2(1.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, 1.0, 1.0),		vec3(-1.0, 0.0, 0.0),	vec2(1.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, 1.0, -1.0),	vec3(-1.0, 0.0, 0.0),	vec2(0.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },

		//upper
		{ vec3(1.0, 1.0, 1.0),		vec3(0.0, 1.0, 0.0),	vec2(1.0f, 1.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, 1.0, 1.0),		vec3(0.0, 1.0, 0.0),	vec2(0.0f, 1.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, 1.0, -1.0),	vec3(0.0, 1.0, 0.0),	vec2(0.0f, 0.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(1.0, 1.0, -1.0),		vec3(0.0, 1.0, 0.0),	vec2(1.0f, 0.0f),	vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },

		//bottom
		{ vec3(-1.0, -1.0, -1.0),	vec3(0.0, -1.0, 0.0),	vec2(0.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(1.0, -1.0, -1.0),	vec3(0.0, -1.0, 0.0),	vec2(1.0f, 0.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(1.0, -1.0, 1.0),		vec3(0.0, -1.0, 0.0),	vec2(1.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-1.0, -1.0, 1.0),	vec3(0.0, -1.0, 0.0),	vec2(0.0f, 1.0f),	vec3(0.0, 0.0, 1.0),	vec3(0.0, 0.0, 1.0) },
	};

	//		 front & back				   right & left				   upper & bottom
	//		  
	//     11 *--------* 10			     15 *        * 6			     18 *--------* 19
	//		  |        | 					|\       |\						 \        \
	//		  |3*--------* 2			  14| *      | * 7				   17 *--------* 16
	//		  | |      | |					| |      | |					           
	//		8 *-|------*9|  			 12 * |      *5| 				 20 *--------* 21
	//		    |        |					 \|       \|					 \        \
	//			*--------*				      *        *				      *--------*
	//		   0          1				    13          4				    22          23

	vector<GLuint> indices = {

		0, 1, 2, 0, 2, 3,		//front
		5, 4, 7, 5, 7, 6,		//right
		10, 9, 8, 10, 8, 11,	//back
		12, 13, 14, 12, 14, 15, //left
		16, 19, 17, 17, 19, 18, //upper
		20, 21, 22, 20, 22, 23	//bottom*/
	};
	
	return Mesh("cube", vertices, indices, nullptr);
}

Mesh generateSphere(const float & radius, const unsigned int & slices, const unsigned int & stacks)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;

	// Generate positions and normals
	float theta, phi;
	float thetaFac = glm::two_pi<float>() / slices;
	float phiFac = glm::pi<float>() / stacks;
	float nx, ny, nz, s, t;
	unsigned int idx = 0, tIdx = 0;
	for (unsigned int i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (float)i / slices;
		for (unsigned int j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (float)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);

			Vertex vertex;
			//position
			vertex.position.x = radius * nx;
			vertex.position.y = radius * ny;
			vertex.position.z = radius * nz;

			//normal
			vertex.normal.x = nx;
			vertex.normal.y = ny;
			vertex.normal.z = nz;

			//texcoord
			vertex.texCoords.s = s;
			vertex.texCoords.t = t;

			vertices.push_back(vertex);
		}
	}

	// Generate the element list
	idx = 0;
	for (unsigned int i = 0; i < slices; i++) {
		unsigned int stackStart = i * (stacks + 1);
		unsigned int nextStackStart = (i + 1) * (stacks + 1);
		for (unsigned int j = 0; j < stacks; j++) {
			if (j == 0) {
				indices.push_back(stackStart);
				indices.push_back(stackStart + 1);
				indices.push_back(nextStackStart + 1);
			}
			else if (j == stacks - 1) {
				indices.push_back(stackStart + j);
				indices.push_back(stackStart + j + 1);
				indices.push_back(nextStackStart + j);
			}
			else {
				indices.push_back(stackStart + j);
				indices.push_back(stackStart + j + 1);
				indices.push_back(nextStackStart + j + 1);
				indices.push_back(nextStackStart + j);
				indices.push_back(stackStart + j);
				indices.push_back(nextStackStart + j + 1);
			}
		}
	}

	return Mesh("sphere", vertices, indices, nullptr);
}

Mesh generateTorus(const int &sides, const int &rings, const float &outerRadius, const float &innerRadius)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;

	float TWOPI = 2 * glm::pi<float>();

	float ringFactor = (float)(TWOPI / rings);
	float sideFactor = (float)(TWOPI / sides);
	int idx = 0, tidx = 0;
	for (int ring = 0; ring <= rings; ring++) {
		float u = ring * ringFactor;
		float cu = cos(u);
		float su = sin(u);
		for (int side = 0; side < sides; side++) {
			float v = side * sideFactor;
			float cv = cos(v);
			float sv = sin(v);
			float r = (outerRadius + innerRadius * cv);

			Vertex vertex;
			//positions
			vertex.position.x = r * cu;
			vertex.position.y = r * su;
			vertex.position.z = innerRadius * sv;

			//normals
			float nx = cv * cu * r;
			float ny = cv * su * r;
			float nz = sv * r;
			float len = sqrt(nx * nx + ny * ny + nz * nz);
			vertex.normal.x = nx / len;
			vertex.normal.y = ny / len;
			vertex.normal.z = nz / len;


			//texcoord
			//TODO : MISSING

			vertices.push_back(vertex);
		}
	}

	for (int ring = 0; ring < rings; ring++) {
		int ringStart = ring * sides;
		int nextRingStart = (ring + 1) * sides;
		for (int side = 0; side < sides; side++) {
			int nextSide = (side + 1) % sides;
			//indices
			indices.push_back(ringStart + side);
			indices.push_back(nextRingStart + side);
			indices.push_back(nextRingStart + nextSide);
			indices.push_back(ringStart + side);
			indices.push_back(nextRingStart + nextSide);
			indices.push_back(ringStart + nextSide);
		}
	}

	return Mesh("torus", vertices, indices, nullptr);
}

Mesh generateQuad(const unsigned int & size)
{
	vec3 normalPlane = vec3(0, 1, 0);
	float fsize = (float)size;

	//
	//  2 *------* 3
	//    |      |
	//    |      |
	//  1 *------* 0
	//   


	vector<Vertex> vertices = {
		{ vec3(fsize, 0, fsize),		normalPlane,	vec2(1.0f, 1.0f), vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-fsize, 0, fsize),		normalPlane,	vec2(0.0f, 1.0f), vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(-fsize, 0, -fsize),		normalPlane,	vec2(0.0f, 0.0f), vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
		{ vec3(fsize, 0, -fsize),		normalPlane,	vec2(1.0f, 0.0f), vec3(1.0, 0.0, 0.0),	vec3(0.0, 0.0, 1.0) },
	};

	vector<GLuint> indices = { 0, 3, 1, 1, 3, 2};

	return Mesh("quad", vertices, indices, nullptr);
}

SimplifiedMesh genSimpleCube(float size)
{
	SimplifiedMesh cube;
	size = abs(size);

	cube.vertices = {
		//front
		-size, -size, size,
		size, -size, size,
		size, size, size,
		-size, size, size,

		//right
		size, size, size,
		size, size, -size,
		size, -size, -size,
		size, -size, size,

		//back
		-size, -size, -size,
		size, -size, -size,
		size, size, -size,
		-size, size, -size,

		//left
		-size, -size, -size,
		-size, -size, size,
		-size, size, size,
		-size, size, -size,

		//upper
		size, size, size,
		-size, size, size,
		-size, size, -size,
		size, size, -size,

		//bottom
		-size, -size, -size,
		size, -size, -size,
		size, -size, size,
		-size, -size, size
	};

	cube.indices = {
		0, 1, 2, 0, 2, 3,		//front
		5, 4, 7, 5, 7, 6,		//right
		10, 9, 8, 10, 8, 11,	//back
		12, 13, 14, 12, 14, 15, //left
		16, 19, 17, 17, 19, 18, //upper
		20, 21, 22, 20, 22, 23	//bottom
	};

	return cube;
}



/*
Mesh generatePlane(const size_t& size)
{
	vec3 normalPlane = vec3(0, 1, 0);
	float fsize = (float)size;

	vector<Vertex> vertices;
	vector<GLuint> indices;

	//			 2 		
	//  0 *------*------* 4
	//    |      |      |
	//    |      |      |
	//  1 *------*------* 5
	//           3

	int currentX, currentY = -(size / 2);

	for (size_t y = 0; y < size; ++y)
	{
		currentY += y;
		currentX = -(size / 2);
		for (size_t x = 0; x < size; ++x)
		{

			Vertex vertex;

			vertex.position = vec3(currentX, 0, currentY);
			vertex.normal = normalPlane;


			float 
			vertex.texCoords = 
		
		}
	}



	vector<Vertex> vertices = {
		{ vec3(fsize, 0, fsize),		normalPlane,	vec2(1.0f, 1.0f), },
		{ vec3(-fsize, 0, fsize),		normalPlane,	vec2(0.0f, 1.0f), },
		{ vec3(-fsize, 0, -fsize),		normalPlane,	vec2(0.0f, 0.0f), },
		{ vec3(fsize, 0, -fsize),		normalPlane,	vec2(1.0f, 0.0f), },
	};

	vector<GLuint> indices = { 0, 3, 1, 1, 3, 2 };

	return Mesh("quad", vertices, indices, nullptr);
}
//*/