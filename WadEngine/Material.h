#pragma once

#include "GL/glew.h"
#include <GL/gl.h>
#include <string>
#include <glm\glm.hpp>

#include "Texture.hpp"
#include "Shader.h"

using namespace std;
using namespace glm;


#define MAX_DIFFUSE_TEX 2
#define MAX_SPECULAR_TEX 1


#define ATTRIB_DIFFUSE	"material.diffuseMap"
#define ATTRIB_SPECULAR "material.specularMap"
#define ATTRIB_NORMAL	"material.normalMap"
#define ATTRIB_GLOW		"material.glowMap"
#define ATTRIB_HAS_NORMAL "material.hasNormalMap"

class Material
{

private:
	std::string	_name;
	Shader*		_shader;

	vector<GLuint> diffuseTextures;
	vector<GLuint> specularTextures;
	GLuint			_normalMap;
	GLuint			_glowMap;

	vec3		_ambiant;
	vec3		_diffuse;
	vec3		_specular;
	float		_shininess;
	vec2		_tiling;

public:
	Material(const std::string& name, Shader *shader);
	~Material();

	void sendInfoToShader();


	//setters
	void setShader(Shader* shader);
	void setAmbiant(const vec3 &color);
	void setDiffuse(const vec3 &color);
	void setSpecular(const vec3 &color);
	void setShinisess(const float &value);
	void setTiling(const vec2 &tiling);

	void setDiffuseTextures(const vector<Texture*> &diffuseMap);
	void setSpecularTextures(const vector<Texture*> &specularMap);
	void setNormalTextures(const vector<Texture*> &normalTexture);

	void setTexture(const Texture* texture, const Texture::eTextureType& type, const unsigned int &count);

	void setMainTexture(const Texture* texture);
	void setMainTexture(const GLuint& textureID);
	void setSecondaryTexture(const Texture* texture);
	void setNormalTexture(const Texture* texture);
	void setNormalTexture(const GLuint& textureID);
	void setGlowMap(const GLuint& textureID);

	//getters
	Shader* getShader();
	const std::string& getName();
	const vec3& getAmbiant();
	const vec3& getDiffuse();
	const vec3& getSpecular();
	const vec2& getTiling();
	const float& getShininess();
	GLuint mainDiffuseTexture() const;
	GLuint secDiffuseTexture() const;
	GLuint normalMap() const;
	GLuint specularMap() const;
	GLuint glowMap() const;
};

