#include "ModelLoading.h"

#include "CatalogueUtils.h"

ModelLoader::ModelLoader(AssetCatalogue * assets) : _assets(assets)
{
}

ModelLoader::~ModelLoader()
{
	_nameUsed.clear();
}

Model * ModelLoader::loadModel(const string & path)
{
	directory = getDirectoryOfPath(path);
	unamedObjectIndex = 0;
	_nameUsed.clear();

	//read the model file with Assimp
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return nullptr;
	}

	//parse Assimp nodes and construct model

	Model* loaded = processNode(scene->mRootNode, scene);
	//if (loaded->name == "")
	loaded->name = getFilename(path);
	return loaded;
}

Model * ModelLoader::processNode(aiNode * node, const aiScene * scene)
{
	Model* model = new Model();

	//process the mesh of the node (if existing)
	if (node->mNumMeshes == 0)
		model->name = "gameobject (" + to_string(++unamedObjectIndex) + ")";
	if (node->mNumMeshes > 0) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
		model->name = mesh->mName.C_Str();
		if (nameUsed(mesh->mName.C_Str()))
			model->name += string(" (" + to_string(++unamedObjectIndex) + ")");
		_nameUsed.push_back(model->name);
		model->mesh = processMesh(mesh, scene);
	}
	if (node->mNumMeshes > 1) //should never happen
		cerr << "[WARNING] One or more mesh of the node were not loaded" << endl;

	// process the children of the node
	for (GLuint i = 0; i < node->mNumChildren; i++)
		model->childs.push_back(processNode(node->mChildren[i], scene));
	return model;
}

Mesh * ModelLoader::processMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;



	// Process vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.bitTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		vertex.texCoords = (mesh->mTextureCoords[0]) ?
			glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process material on the mesh
	Material *material = processMaterial(mesh, scene);

	return new Mesh(mesh->mName.C_Str(), vertices, indices, material);
}

Material * ModelLoader::processMaterial(aiMesh * mesh, const aiScene * scene)
{
	//check if at least one material is attached to the mesh
	if (mesh->mMaterialIndex < 0) return nullptr;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	aiString name;
	material->Get(AI_MATKEY_NAME, name);

	//check if material (same name) was already created
	//TODO : reduce the search only for the current model
	pair<bool, Material*> tmp = _assets->hasMaterial(name.C_Str());
	if (tmp.first) return tmp.second;


	//create the new Material 
	Material* newMaterial = _assets->addMaterial(name.C_Str());
	
	aiColor3D color(0.f, 0.f, 0.f);
	float value;

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		newMaterial->setDiffuse(vec3(color.r, color.g, color.b));

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
		newMaterial->setAmbiant(vec3(color.r, color.g, color.b));

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		newMaterial->setSpecular(vec3(color.r, color.g, color.b));

	if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, value))
		newMaterial->setShinisess(value);

	vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::DIFFUSE);
	newMaterial->setDiffuseTextures(diffuseMaps);

	vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::SPECULAR);
	newMaterial->setSpecularTextures(specularMaps);

	vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::NORMAL);
	newMaterial->setNormalTextures(normalMaps);
	
	return newMaterial;
}

vector<Texture*> ModelLoader::loadMaterialTextures(aiMaterial * mat, aiTextureType type, Texture::eTextureType typeName)
{
	vector<Texture*> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture* texture = _assets->addTexture(str.C_Str(), directory + str.C_Str());
		if (!texture) continue;
		texture->type = typeName;
		textures.push_back(texture);
	}
	return textures;
}

bool ModelLoader::nameUsed(const string & name)
{
	for (auto used : _nameUsed)
		if (used == name) return true;
	return false;
}


/*
Model * loadModel(AssetCatalogue* assetCatalogue, const string & path)
{
	string directory = getDirectoryOfPath(path);

	//read the model file with Assimp
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace
	);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return nullptr;
	}

	//parse Assimp nodes and construct model

	Model* loaded = processNode(assetCatalogue, scene->mRootNode, scene, directory);
	if (loaded->name == "")
		loaded->name = getFilename(path);
	return loaded;
}

Model * processNode(AssetCatalogue* assetCatalogue, aiNode * node, const aiScene * scene, const string &directory)
{
	static int unamedObjectIndex = 0;
	Model* model = new Model();

	//process the mesh of the node (if existing)
	if (node->mNumMeshes > 0) {

		aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
		model->name = mesh->mName.C_Str();
		if (model->name == "")
			model->name += "gameobject " + unamedObjectIndex;
		else
			cout << "load mesh [" << model->name << "]" << endl;
		model->mesh = processMesh(assetCatalogue, mesh, scene, directory);
	}
	if (node->mNumMeshes > 1) //should never happen
		cerr << "[WARNING] One or more mesh of the node were not loaded" << endl;

	// process the children of the node
	for (GLuint i = 0; i < node->mNumChildren; i++)
		model->childs.push_back(processNode(assetCatalogue, node->mChildren[i], scene, directory));
	return model;
}

Mesh * processMesh(AssetCatalogue* assetCatalogue, aiMesh * mesh, const aiScene * scene, const string &directory)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;

	Material *mat = assetCatalogue->addMaterial(mesh->mName.C_Str());
	
	// Process vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.bitTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		vertex.texCoords = (mesh->mTextureCoords[0]) ?
			glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture*> diffuseMaps = loadMaterialTextures(assetCatalogue, material, aiTextureType_DIFFUSE, Texture::DIFFUSE, directory);
		mat->setDiffuseTextures(diffuseMaps);

		vector<Texture*> specularMaps = loadMaterialTextures(assetCatalogue, material, aiTextureType_SPECULAR, Texture::SPECULAR, directory);
		mat->setSpecularTextures(specularMaps);

		vector<Texture*> normalMaps = loadMaterialTextures(assetCatalogue, material, aiTextureType_HEIGHT, Texture::NORMAL, directory);
		mat->setNormalTextures(normalMaps);
	}

	return new Mesh(mesh->mName.C_Str(), vertices, indices, mat);
}

vector<Texture*> loadMaterialTextures(AssetCatalogue* assetCatalogue, aiMaterial * mat, aiTextureType type, Texture::eTextureType typeName, const string &directory)
{
	vector<Texture*> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture* texture = assetCatalogue->addTexture(str.C_Str(), directory + str.C_Str());
		if (!texture) continue;
		texture->type = typeName;
		textures.push_back(texture);
	}
	return textures;
}
*/