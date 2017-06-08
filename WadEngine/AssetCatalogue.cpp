#include "AssetCatalogue.h"
#include <iostream>

#include "ShaderLoading.h"
#include "TextureLoading.h"
#include "ModelLoading.h"
#include "CatalogueUtils.h"

GLuint AssetCatalogue::whiteTexture = INVALID_TEXTURE_ID;

AssetCatalogue::AssetCatalogue() {}

void AssetCatalogue::initEngineAssets()
{
	cout << "==== LOAD ENGINE ASSETS ======" << endl;

	whiteTexture = loadColorTexture((char)255, (char)255, (char)255, (char)0);
	
	addShader("standard",		SHADER_FOLDER("StandardVertShader.glsl"),	SHADER_FOLDER("StandardFragShader.glsl"));
	addShader("screen",			SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("ScreenFragShader.glsl"));
	addShader("hdr",			SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("HDR_frag.glsl"));
	addShader("bloom",			SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("bloom_frag.glsl"));
	addShader("blur",			SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("blur_frag.glsl"));
	addShader("greyFilter",		SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("GreyFilterFragShader.glsl"));
	addShader("sepiaFilter",	SHADER_FOLDER("ScreenVertShader.glsl"),		SHADER_FOLDER("SepiaFilterFragShader.glsl"));
	addShader("skybox",			SHADER_FOLDER("CubeMapVertShader.glsl"),	SHADER_FOLDER("CubeMapFragShader.glsl"));
	addShader("singleColor",	SHADER_FOLDER("SimpleVertShader.glsl"),		SHADER_FOLDER("SingleColorFragShader.glsl"));
	
	vector<string> skyboxFaces = {
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetRight2048.png"),
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetLeft2048.png"),
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetUp2048.png"),
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetDown2048.png"),
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetBack2048.png"),
		TEXTURE_FOLDER("Skybox1/SunSet/SunSetFront2048.png"),
	};
	addCubeMap("defaultSkybox", skyboxFaces);

	addMaterial("default"); //default material

	cout << "==== LOADED ALL ENGINE ASSETS ======" << endl;
}

AssetCatalogue::~AssetCatalogue()
{
	for (auto texture : _textures)
		delete texture;
	for (auto cubemap : _cubeMaps)
		delete cubemap;
	for (auto material : _materials)
		delete material;
	for (auto shader : _shaders)
		delete shader;
	for (auto model : _models)
		delete model;

	//cout << "Deleted all assets" << endl;
}

Shader* AssetCatalogue::addShader(const string &name, const string& vertShaderPath, const string& fragShaderPath)
{
	GLuint programId = loadShader(vertShaderPath.c_str(), fragShaderPath.c_str());
	if (programId == INVALID_SHADER_ID) return nullptr;

	//TODO : parser les attributs des Shaders

	_shaders.push_back(new Shader(programId, name, vertShaderPath, fragShaderPath));

	cout << "Loaded Shader [" << name << "]" << endl;

	return _shaders.back();
}

Texture* AssetCatalogue::addTexture(const std::string& name, const std::string& path)
{
	//check if texture was already loaded
	for (auto it = _textures.begin(); it != _textures.end(); it++) 
	{
		if ((*it)->path == path)
			return *it;
	}

	GLuint textureID = loadTexture(path.c_str());
	if (textureID == INVALID_TEXTURE_ID) return nullptr;

	_textures.push_back(new Texture(path, name, textureID));

	cout << "Load Texture [" << name << "]" << endl;

	return _textures.back();
}

Texture * AssetCatalogue::addTexture(const string & path)
{
	//check if texture was already loaded
	for (auto loaded : _textures)
		if (loaded->path == path) return loaded;

	GLuint textureID = loadTexture(path.c_str());
	if (textureID == INVALID_TEXTURE_ID) return nullptr;

	_textures.push_back(new Texture(path, getFilename(path), textureID));

	return _textures.back();
}

Model* AssetCatalogue::addModel(const string& path)
{
	//check if model was already loaded
	for (auto it = _models.begin(); it != _models.end(); it++)
	{
		if (path == (*it)->path) {
			cout << "Model already loaded" << endl;
			return *it;
		}
	}
	ModelLoader loader(this);

	Model * newModel = loader.loadModel(path);
	if (newModel == nullptr) return nullptr;

	newModel->path = path;
	_models.push_back(newModel);

	cout << "= Load Model [" << path << "]" << endl;

	return _models.back();
}

Material * AssetCatalogue::addMaterial(const string& name)
{
	//TODO : save material in file
	//@Incomplete : check if name is not already used for another material
	Material* newMaterial = new Material(name, defaultShader());
	_materials.push_back(newMaterial);
	return newMaterial;
}

CubeMap * AssetCatalogue::addCubeMap(const string& name, vector<string>& textures)
{
	GLuint cubemapID = loadCubeMap(textures);
	if (cubemapID == INVALID_TEXTURE_ID) return nullptr;

	CubeMap* cubemap = new CubeMap(cubemapID, name, textures);
	_cubeMaps.push_back(cubemap);
	return cubemap;
}




Shader* AssetCatalogue::shader(const string & name)
{
	for (auto it = _shaders.begin(); it != _shaders.end(); it++) {
		if ((*it)->name() == name)
			return (*it);
	}
	return defaultShader();
}
Texture * AssetCatalogue::texture(const string & name)
{
	for (auto it = _textures.begin(); it != _textures.end(); it++) {
		if ((*it)->name == name)
			return (*it);
	}
	return nullptr;
}
Texture * AssetCatalogue::texture(const GLuint & id)
{
	for (auto texture : _textures) 
		if (texture->id == id) return texture;
	return nullptr;
}
Model * AssetCatalogue::model(const string & name)
{
	for (auto it = _models.begin(); it != _models.end(); it++) {
		if ((*it)->name == name)
			return (*it);
	}
	return nullptr;
}
Material * AssetCatalogue::material(const string & name)
{
	for (auto it = _materials.begin(); it != _materials.end(); it++) {
		if ((*it)->getName() == name)
			return (*it);
	}
	return nullptr;
}

CubeMap * AssetCatalogue::defaultSkyBox()
{
	return _cubeMaps[0];
}

Shader* AssetCatalogue::defaultShader() 
{
	return _shaders[0];
}
Material * AssetCatalogue::defaultMaterial()
{
	return _materials[0];
}

pair<bool, Material*> AssetCatalogue::hasMaterial(const string & name)
{
	for (auto material : _materials)
	{
		if (material->getName() == name)
		{
			return make_pair(true, material);
		}
	}

	return make_pair(false, nullptr);
}

const vector<Material*>& AssetCatalogue::materials() const { return _materials; }

const vector<Texture*>& AssetCatalogue::textures() const { return _textures; }
