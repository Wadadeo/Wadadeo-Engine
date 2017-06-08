#pragma once

#include <vector>
#include "RendererComponent.h"
#include "LightComponent.h"
#include "AssetCatalogue.h"
#include "Camera.h"
#include "SkyboxRenderer.h"
#include "LightSystem.h"
#include "BloomEffect.h"

using namespace std;

#define MAX_RENDERER_COMPONENT 256

class GraphicsSystem
{
	LightSystem		_lightSystem;
	vector<RendererComponent*> _toRender;


	AssetCatalogue* _assets;	

	Camera			camera;
	SkyboxRenderer	_skyboxRenderer;
	BloomEffect		_bloom;
	vec3			clearColor;
	GLenum			drawMode;

	Shader*			_effect;

	int		width;
	int		height;
	GLuint	FBO;
	GLuint	colorBuffers[2];
	GLuint	screenVAO;

	
public:
	static float exposure;
	bool useHDR = false;

	GraphicsSystem();
	~GraphicsSystem();
	void init(int screenWidth, int screenHeight, AssetCatalogue * assets);
	void update(float deltaTime);

	void add(RendererComponent* cmp);
	void remove(RendererComponent* cmp);
	
	void useEffect(const std::string & effectName);
	void resize(int width, int height);

	Shader* effect();
	float* getClearColor();
	SkyboxRenderer& skyboxRenderer();
	LightSystem& lightSystem();
	BloomEffect& bloomEffect();


private:
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	void setupScreenTexture();
	void setupFrameBuffer();
};

