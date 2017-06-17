#include "Material.h"
#include "glm/gtc/type_ptr.hpp"

#include "AssetCatalogue.h"

Material::Material(const std::string & name, Shader *shader) : 
	_name(name), _shader(shader)
{
	_ambiant = vec3(0.1f, 0.1f, 0.1f);
	_diffuse = vec3(0.8, 0.8, 0.8);
	_specular = _diffuse;
	_shininess = 16.0f;
	_tiling = vec2(1, 1);

	for (int i = 0; i < MAX_DIFFUSE_TEX; i++)
		diffuseTextures.push_back(AssetCatalogue::whiteTexture);
	for (int i = 0; i < MAX_SPECULAR_TEX; i++)
		specularTextures.push_back(AssetCatalogue::whiteTexture);
	_normalMap = AssetCatalogue::whiteTexture;
	_glowMap = 0;
}

Material::~Material() {}


void Material::sendInfoToShader()
{
	//glUniform3fv(glGetUniformLocation(_shader->getProgram(), "material.ambient"), 1, glm::value_ptr(_ambiant));
	glUniform3fv(glGetUniformLocation(_shader->program(), "material.diffuse"), 1, glm::value_ptr(_diffuse));
	glUniform3fv(glGetUniformLocation(_shader->program(), "material.specular"), 1, glm::value_ptr(_specular));
	glUniform1f(glGetUniformLocation(_shader->program(), "material.shininess"), _shininess);

	GLuint diffuseNr = 0;
	GLuint specularNr = 0;
	GLuint i = 0;
	for (i; i < diffuseTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		string name = ATTRIB_DIFFUSE + to_string(++diffuseNr);
		//glUniform1i(glGetUniformLocation(_shader->getProgram(), name.c_str()), i);
		_shader->uniform(name.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, diffuseTextures[i]);
	}

	for (i; i < specularTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		string name = ATTRIB_SPECULAR + to_string(++specularNr);
		_shader->uniform(name.c_str(), i);
//		glUniform1i(glGetUniformLocation(_shader->getProgram(), name.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, specularTextures[i]);
	}

	if (_normalMap != AssetCatalogue::whiteTexture && _normalMap != 0) {
		glActiveTexture(GL_TEXTURE0 + i);
		_shader->uniform(ATTRIB_NORMAL, i++);
		glBindTexture(GL_TEXTURE_2D, _normalMap);
		_shader->uniform(ATTRIB_HAS_NORMAL, true);
	}
	else
		_shader->uniform(ATTRIB_HAS_NORMAL, false);


	glActiveTexture(GL_TEXTURE0 + i);
	_shader->uniform(ATTRIB_GLOW, i++);
	glBindTexture(GL_TEXTURE_2D, _glowMap);

	_shader->uniform("material.tiling", _tiling);

}

//setters

void Material::setShader(Shader * shader) { _shader = shader;  }
void Material::setAmbiant(const vec3 & color) { _ambiant = color; }
void Material::setDiffuse(const vec3 & color) { _diffuse = color; }
void Material::setSpecular(const vec3 & color) { _specular = color; }
void Material::setShinisess(const float & value) { _shininess = value; }
void Material::setTiling(const vec2 & tiling) { _tiling = tiling; }

void Material::setDiffuseTextures(const vector<Texture*>& diffuseMap)
{
	for (size_t i = 0; i < MAX_DIFFUSE_TEX && i < diffuseMap.size(); i++) {
		diffuseTextures[i] = diffuseMap[i]->id;
	}
}
void Material::setSpecularTextures(const vector<Texture*>& specularMap)
{
	for (size_t i = 0; i < MAX_SPECULAR_TEX && i < specularMap.size(); i++) {
		specularTextures[i] = specularMap[i]->id;
	}
}
void Material::setNormalTextures(const vector<Texture*>& normalTextures)
{
	//there sould be only one normal map
	if (normalTextures.size() > 0)
		_normalMap = normalTextures[0]->id;
}

void Material::setTexture(const Texture * texture, const Texture::eTextureType & type, const unsigned int &count)
{
	vector<GLuint>& textureMap = diffuseTextures;
	unsigned int limit = MAX_DIFFUSE_TEX;
	if (type == Texture::SPECULAR) {
		textureMap = specularTextures;
		limit = MAX_SPECULAR_TEX;
	}
	if (count < limit)
		textureMap[count] = texture->id;
}
void Material::setMainTexture(const Texture* texture) {
	setTexture(texture, Texture::DIFFUSE, 0);
}
void Material::setMainTexture(const GLuint & textureID)
{
	diffuseTextures[0] = textureID;
}
void Material::setSecondaryTexture(const Texture* texture)
{
	setTexture(texture, Texture::DIFFUSE, 1);
}

void Material::setNormalTexture(const Texture * texture)
{
	_normalMap = texture->id;
}

void Material::setNormalTexture(const GLuint & textureID)
{
	_normalMap = textureID;
}

void Material::setGlowMap(const GLuint & textureID)
{
	_glowMap = textureID;
}

//getters

Shader * Material::getShader(){ return _shader; }
const std::string & Material::getName() { return _name; }
const vec3 & Material::getAmbiant() { return _ambiant; }
const vec3 & Material::getDiffuse() { return _diffuse; }
//const GLuint & Material::getDiffuseTexture() { return _diffuseTexture; }
const vec3 & Material::getSpecular() { return _specular; }
const vec2 & Material::getTiling() { return _tiling; }
const float & Material::getShininess() { return _shininess; }

GLuint Material::mainDiffuseTexture() const
{
	if (diffuseTextures.size() > 0) return diffuseTextures[0];
	return 0;
}

GLuint Material::secDiffuseTexture() const
{
	return GLuint();
}

GLuint Material::normalMap() const
{
	return _normalMap;
}

GLuint Material::specularMap() const
{
	return GLuint();
}

GLuint Material::glowMap() const
{
	return _glowMap;
}



