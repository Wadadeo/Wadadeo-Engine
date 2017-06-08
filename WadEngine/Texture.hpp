#pragma once

#include <string>

#include "GL/glew.h"
#include <GL/gl.h>

using namespace std;

class Texture {
public:

	enum eTextureType {
		UNDEFINED,
		DIFFUSE,
		SPECULAR,
		NORMAL,
		CUBEMAP
	};

	GLuint			id;
	std::string		name;
	std::string		path;
	eTextureType	type;
	//unsigned short	width;
	//unsigned short	height;

	Texture(const string &path, const string &alias, const GLuint& textureID) :
		path(path), name(alias), id(textureID), type(UNDEFINED)
	{}

};