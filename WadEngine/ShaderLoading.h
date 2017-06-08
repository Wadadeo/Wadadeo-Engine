#pragma once

#include <string>
#include "Shader.h"

#define INVALID_SHADER_ID 0

GLuint loadShader(const char * vertShaderPath, const char * fragShaderPath);
GLuint compileShader(const std::string &sourceCode, GLenum type);