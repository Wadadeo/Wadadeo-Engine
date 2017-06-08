#pragma once

#include "GL/glew.h"
#include <GL/gl.h>

#include "Shader.h"


enum eBloomDebug
{
	COMPLETE = 0,
	SCENE = 1,
	BRIGHT_ONLY = 2,
	BLURRED_BRIGHT = 3
};

class BloomEffect
{

	//bloom FBO
	GLuint hdrFBO; 
	GLuint colorBuffers[2]; //2 color attachement : 1 for normal & 1 for bright area

	//Gaussian blur FBO
	GLuint blurFBOs[2];
	GLuint blurColorbuffers[2];

	Shader* _bloom;
	Shader* _blur;
	Shader* _hdr;

public:

	eBloomDebug debug = COMPLETE;
	float exposure = 1.0f;

	BloomEffect();
	~BloomEffect();

	void init(int screenWidth, int screenHeight, Shader* bloom, Shader * blur, Shader *hdr);
	void apply(GLuint screenVAO);

	GLuint frameBuffer() const;

private:
	void setupGaussianBlurFBOs(int screenWidth, int screenHeight);
};

