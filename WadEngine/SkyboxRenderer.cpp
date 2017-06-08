#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer() {}

SkyboxRenderer::SkyboxRenderer(CubeMap * cubemap, Shader * skyboxShader) :
	_cubemap(cubemap), _shader(skyboxShader)
{
	cout << "SkyboxRenderer sec constructor called" << endl;

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), &cubeMapVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

SkyboxRenderer::SkyboxRenderer(const SkyboxRenderer & other)
{
	cout << "SkyboxRenderer copy constructor called" << endl;
	enabled = other.enabled;
	_shader = other._shader;
	_cubemap = other._cubemap;
	_VAO = other._VAO;
	_VBO = other._VBO;
}


SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::draw(Camera &camera)
{
	glDepthMask(GL_FALSE);
	_shader->use();
	// ... Set view and projection matrix
	_shader->uniform(VIEW_MATRIX, glm::mat4(glm::mat3(camera.getViewMatrix()))); //converting to mat3 and then mat4 remove translations
	_shader->uniform(PROJECTION_MATRIX, camera.getProjectionMaxtrix());

	_shader->uniform("skybox", (GLuint)0);
	glBindVertexArray(_VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap->id());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	/*glDrawElements(
		GL_TRIANGLES,						// mode
		meshCpt->elementCount(),			// count
		GL_UNSIGNED_INT,					// type
		0									// element array buffer offset
	);*/
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

const CubeMap * SkyboxRenderer::cubeMap() const
{
	return _cubemap;
}

GLfloat SkyboxRenderer::cubeMapVertices[108] =
{
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};