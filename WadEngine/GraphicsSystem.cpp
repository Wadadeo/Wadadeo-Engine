#include "GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "GameObject.h"

GraphicsSystem* RendererComponent::system = nullptr;
float GraphicsSystem::exposure = 1.0f;

GraphicsSystem::GraphicsSystem()
{
	RendererComponent::system = this;

	clearColor.r = 0.05f;
	clearColor.g = 0.05f;
	clearColor.b = 0.05f;
	_toRender.reserve(MAX_RENDERER_COMPONENT);
}


GraphicsSystem::~GraphicsSystem()
{
}

void GraphicsSystem::init(int screenWidth, int screenHeight, AssetCatalogue* assets)
{
	width = screenWidth;
	height = screenHeight;
	
	camera.useSceenDimensions(width, height);
	
	_assets = assets;

	_effect = _assets->shader("screen");
	assert(_effect != nullptr);

	_skyboxRenderer = SkyboxRenderer(_assets->defaultSkyBox(), _assets->shader("skybox"));
	_lightSystem.init(_assets->shader("singleColor"));

	_bloom.init(screenWidth, screenHeight, _assets->shader("bloom"), _assets->shader("blur"), _assets->shader("hdr"));

	drawMode = GL_FILL;
	//drawMode = GL_LINE;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	//avoid back face drawing
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	setupFrameBuffer();
	setupScreenTexture();
}

void GraphicsSystem::update(float deltaTime)
{
	camera.update(deltaTime);
	
	// 1 - Render the scene in a frameBuffer
	glPolygonMode(GL_FRONT_AND_BACK, drawMode);
	glBindFramebuffer(GL_FRAMEBUFFER, _bloom.frameBuffer()); //use bloom framebuffer      
		glClearColor(clearColor.x, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // if using stencil, remember to add it here
		glEnable(GL_DEPTH_TEST);

		if (_skyboxRenderer.enabled)
			_skyboxRenderer.draw(camera);
		_lightSystem.update(camera);
		for (auto it = _toRender.begin(); it != _toRender.end(); it++)
			(*it)->draw(camera);

	
	// 2 - Apply effect on the texture rendered with previous FBO and draw it on screen
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //not necessary, quad is "fullscreen"
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		// Draw Screen texture (quad)

		_bloom.apply(screenVAO);

		/*_effect->use();

		if (useHDR)
		{
			//_effect->uniform("gamma", 2.2f);
			_effect->uniform("exposure", exposure);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(screenVAO);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);	// Use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);*/

}

void GraphicsSystem::useEffect(const std::string & effectName)
{
	useHDR = false;
	//cout << "-> Now use image effect : " << (effectName ? effectName : "default") << endl;
	if (effectName != "")
	{		
		_effect = _assets->shader(effectName);
		if (effectName == "hdr")
			useHDR = true;
	}



	if (!_effect || effectName == "")
		_effect = _assets->shader("screen");
}

void GraphicsSystem::add(RendererComponent * cmp)
{
	_toRender.push_back(cmp);
}

void GraphicsSystem::remove(RendererComponent * cmp)
{
	for (auto it = _toRender.begin(); it != _toRender.end(); ++it)
	{
		if (*it == cmp)
		{
			if (_toRender.size() >= 2)
			{
				std::swap(*it, _toRender.back());
				_toRender.pop_back();
			}
			else
			{
				_toRender.clear();
			}
			break;
		}
	}
}

void GraphicsSystem::resize(int w, int h)
{
	width = w;
	height = h;
}

Shader * GraphicsSystem::effect() { return _effect; }

float* GraphicsSystem::getClearColor() { return glm::value_ptr(clearColor); }
SkyboxRenderer & GraphicsSystem::skyboxRenderer() { return _skyboxRenderer; }
LightSystem & GraphicsSystem::lightSystem() { return _lightSystem; }

BloomEffect & GraphicsSystem::bloomEffect()
{
	return _bloom;
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint GraphicsSystem::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGBA16F; //GL_RGB for normal color buffer
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void GraphicsSystem::setupScreenTexture()
{
	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// Setup screen VAO
	GLuint quadVBO;
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void GraphicsSystem::setupFrameBuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create a color attachment textures
	
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

