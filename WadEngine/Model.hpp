#pragma once

#include <vector>
#include <string>
#include "Mesh.hpp"

class Model
{
public:
	string			name;
	string			path;
	Mesh*			mesh;
	vector<Model*>	childs;

	Model() { mesh = nullptr; };
	~Model() 
	{
		for (auto child : childs)
			delete child;
		if (mesh) delete mesh;
	};
};

