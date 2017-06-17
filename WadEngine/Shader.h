#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "GL/glew.h"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"


#define MODEL_MATRIX			"model"
#define VIEW_MATRIX				"view"
#define PROJECTION_MATRIX		"projection"
#define NORMAL_MATRIX			"NormalMatrix"

#define MVP_ATTRIB				"MVP"
#define TEXTURE_ATTRIB			"textureSampler"
#define VERTEX_POSITION_ATTRIB	"vertexPosition"
#define VERTEX_COLOR_ATTRIB		"vertexColor"
#define VERTEX_NORMAL_ATTRIB	"vertexNormal"
#define VERTEX_UV_ATTRIB		"vertexUV"


using namespace std;

class Shader
{
private:
	GLuint				_programID;
	string			_name;
	const string	vertexShader;
	const string	fragShader;


public:
	Shader();
	Shader(const GLuint &programId, const string &name, const string &vertShaderPath, const string& fragShaderPath);
	~Shader();

	void use() const;
	void disable() const;
	void deleteProgram() const;

	void uniform(const char *name, const glm::mat4 &value) const;
	void uniform(const char *name, const glm::mat3 &value) const;
	void uniform(const char *name, const glm::vec3 &value) const;
	void uniform(const char *name, const glm::vec2 &value) const;
	void uniform(const char *name, const unsigned int &value) const;
	void uniform(const char *name, const float &value) const;
	void uniform(const char *name, bool value) const;

	const GLuint& program() const;
	const string& name() const;
	const string& vertShaderPath() const;
	const string& fragShaderPath() const;

};

