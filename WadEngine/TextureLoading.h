#pragma once

#include <vector>
#include <string>
#include "GL/glew.h"
#include <GL/gl.h>

#define INVALID_TEXTURE_ID 0


using namespace std;

GLuint loadTexture(const char* texturePath);
GLuint loadColorTexture(char r, char g, char b, char a);

GLuint loadCubeMap(vector<string>& texturesPath);
