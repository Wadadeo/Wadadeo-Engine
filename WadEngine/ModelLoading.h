#pragma once

#include <string>
#include <list>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.hpp"
#include "AssetCatalogue.h"


using namespace std;



class ModelLoader
{
	AssetCatalogue* _assets;
	string			directory;
	list<string>	_nameUsed;
	unsigned int unamedObjectIndex;
public:
	ModelLoader(AssetCatalogue* assets);
	~ModelLoader();
	Model* loadModel(const string& path);
private:
	Model* processNode(aiNode * node, const aiScene * scene);
	Mesh* processMesh(aiMesh * mesh, const aiScene * scene);
	Material* processMaterial(aiMesh * mesh, const aiScene * scene);

	vector<Texture*> loadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::eTextureType typeName);
	bool nameUsed(const string& name);
};
