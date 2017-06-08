#pragma once

#include <string>
#include <vector>
#include <utility>

#include "Shader.h"
#include "Texture.hpp"
#include "Model.hpp"
#include "Cubemap.hpp"

using namespace std;

#define TEXTURE_FOLDER(path) string("../Assets/Textures/") + path
#define SHADER_FOLDER(path) string("../Assets/Shaders/") + path

class AssetCatalogue
{
	vector<Shader*>		_shaders;
	vector<Texture*>	_textures;
	vector<Model*>		_models;
	vector<Material*>	_materials;
	vector<CubeMap*>	_cubeMaps;

public:

	static GLuint whiteTexture; //used as default value for sampler2D


	AssetCatalogue();
	~AssetCatalogue();
	
	Shader*		addShader(const string& name, const string& vertShaderPath, const string& fragShaderPath);
	Texture*	addTexture(const std::string& name, const std::string& path);
	Texture*	addTexture(const string& path);
	Model*		addModel(const string& path);
	Material*	addMaterial(const string& name);
	CubeMap*	addCubeMap(const string& name, vector<string>& textures);

	Shader*		shader(const string& name);
	Texture*	texture(const string& name);
	Texture*	texture(const GLuint& id);
	Model*		model(const string& name);
	Material*	material(const string& name);
	
	CubeMap*	defaultSkyBox();
	Shader*		defaultShader();
	Material*	defaultMaterial();


	pair<bool, Material*> hasMaterial(const string& name);

	const vector<Material*>& materials() const;
	const vector<Texture*>& textures() const;

	void initEngineAssets();
};

