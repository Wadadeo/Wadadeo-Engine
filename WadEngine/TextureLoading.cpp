#include "TextureLoading.h"

#include <iostream>

#include <SOIL/SOIL.h>
#include "Texture.hpp"

using namespace std;


GLuint loadTexture(const char* texturePath)
{
	int width, height, channels;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, &channels, SOIL_LOAD_AUTO);
	if (image == 0) {
		cerr << "Failed to load texture [" << texturePath << "] : " << SOIL_last_result() << std::endl;
		return INVALID_TEXTURE_ID;
	}

	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create texture
	GLint textureFormat = (channels == 4 ? GL_RGBA : GL_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, image);
	
	//mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return textureID;
}

//load single color 1x1 texture
GLuint loadColorTexture(char r, char g, char b, char a)
{
	GLuint colorTexID;

	glGenTextures(1, &colorTexID);
	glBindTexture(GL_TEXTURE_2D, colorTexID);
	char color[4] = { r, g, b, a };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return colorTexID;
}

//load cubemap with 6 textures [rigth, left, top, bottom, back, front]
GLuint loadCubeMap(vector<string>& texturesPath)
{
	if (texturesPath.size() < 6)
	{
		cerr << "Loading cubemap need 6 textures. Only " << texturesPath.size() << " were given..." << endl;
		return INVALID_TEXTURE_ID;
	}

	int width, height, channel;
	unsigned char* image;


	GLuint cubemapID;
	glGenTextures(1, &cubemapID);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	for (GLuint i = 0; i < texturesPath.size(); i++)
	{
		image = SOIL_load_image(texturesPath[i].c_str(), &width, &height, &channel, SOIL_LOAD_RGB);
		if (image == 0) {
			cerr << "Failed to load texture [" << texturesPath[i] << "] : " << SOIL_last_result() << std::endl;
			return INVALID_TEXTURE_ID;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
		SOIL_free_image_data(image);
	}

	/*
	GLuint cubemapID = SOIL_load_OGL_cubemap
	(
		texturesPath[0].c_str(),
		texturesPath[1].c_str(),
		texturesPath[2].c_str(),
		texturesPath[3].c_str(),
		texturesPath[4].c_str(),
		texturesPath[5].c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	*/


	//filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);




	return cubemapID;
}
