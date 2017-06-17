#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader()
{
}

Shader::Shader(const GLuint &programId, const std::string &name, const std::string &vertShaderPath, const std::string& fragShaderPath)
	: _programID(programId), _name(name), vertexShader(vertShaderPath), fragShader(fragShaderPath)
{
}

Shader::~Shader() {}




void Shader::use() const { glUseProgram(_programID);}

void Shader::disable() const { glUseProgram(0); }

void Shader::deleteProgram() const { glDeleteProgram(_programID); }



void Shader::uniform(const char * name, const glm::mat4 & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	//else
	//	std::cerr << "Could not set [mat4] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, const glm::mat3 & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	//else
	//	std::cerr << "Could not set [mat3] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, const glm::vec3 & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniform3f(location, value.x, value.y, value.z);
	//else
	//	std::cerr << "Could not set [vec3] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, const glm::vec2 & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniform2f(location, value.x, value.y);
	//else
	//	std::cerr << "Could not set [vec3] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, const unsigned int & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniform1i(location, value);
	//else
	//	std::cerr << "Could not set [int] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, const float & value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniform1f(location, value);
	//else
	//	std::cerr << "Could not set [float] uniform attribute : " << name << std::endl;
}

void Shader::uniform(const char * name, bool value) const
{
	GLint location = glGetUniformLocation(_programID, name);
	if (location != -1)
		glUniform1i(location, value ? 1 : 0);
	//else
	//	std::cerr << "Could not set [bool] uniform attribute : " << name << std::endl;

}


const GLuint & Shader::program() const { return _programID; }
const string & Shader::name() const { return _name; }
const string & Shader::vertShaderPath() const { return vertexShader; }
const string & Shader::fragShaderPath() const { return vertexShader; }