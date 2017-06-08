#include "ShaderLoading.h"
#include "CatalogueUtils.h"

using namespace std;

GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path)
{
	std::string vertexShaderCode = getTextFromFile(vertex_file_path);
	std::string fragmentShaderCode = getTextFromFile(fragment_file_path);

	// Create & complie the shaders from source code
	GLuint vertexShaderID = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);


	// Link the program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);

	// Check link of the program
	GLint programLinkSuccess = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &programLinkSuccess);
	if (programLinkSuccess != GL_TRUE) {
		int InfoLogLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		cerr << &ProgramErrorMessage[0] << endl;
		return INVALID_SHADER_ID;
	}

	glDetachShader(id, vertexShaderID);
	glDetachShader(id, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return id;
}

GLuint compileShader(const std::string & sourceCode, GLenum type)
{
	GLuint shaderId = glCreateShader(type);
	const char *shaderSourcePointer = sourceCode.c_str();

	glShaderSource(shaderId, 1, &shaderSourcePointer, NULL); // Attach source code to Shader
	glCompileShader(shaderId);


	//Check sahder compilation
	GLint shaderStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_FALSE) {
		int InfoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);

		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	return shaderId;
}