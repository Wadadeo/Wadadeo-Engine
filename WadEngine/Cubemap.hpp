#pragma once

#include <string>
#include <vector>

#include "GL/glew.h"

using namespace std;

class CubeMap 
{

	enum eFace {
		RIGHT_FACE = 0,
		LEFT_FACE = 1,
		UP_FACE = 2,
		DOWN_FACE = 3,
		BACK_FACE = 4,
		FRONT_FACE = 5
	};

	GLuint		_id;
	string		_name;
	string		_textures[6];

public:

	CubeMap(const GLuint& cubemapID, const string& name, const vector<string>& textures);
	~CubeMap();

	const GLuint& id() const;
	const string& name() const;
	const string& texture(eFace face) const;
};