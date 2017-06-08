#include "Cubemap.hpp"

CubeMap::CubeMap(const GLuint& cubemapID, const string& name, const vector<string>& textures)
	:_id(cubemapID), _name(name)
{
	for (size_t i = 0; i < 6; ++i)
		_textures[i] = textures[i];
}

CubeMap::~CubeMap() {}

const GLuint& CubeMap::id() const { return _id; }
const string& CubeMap::name() const { return _name; }
const string& CubeMap::texture(eFace face) const { return _textures[face]; }