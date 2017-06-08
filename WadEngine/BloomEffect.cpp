#include "BloomEffect.h"

//float BloomEffect::exposure = 1.0f;

BloomEffect::BloomEffect()
{
}


BloomEffect::~BloomEffect()
{
}

void BloomEffect::init(int screenWidth, int screenHeight, Shader* bloom, Shader * blur, Shader *hdr)
{
	_bloom = bloom;
	_blur = blur;
	_hdr = hdr;
	
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	setupGaussianBlurFBOs(screenWidth, screenHeight);
}

void BloomEffect::apply(GLuint screenVAO)
{
	bool horizontal = true;
	bool first_iteration = true;
	unsigned int amount = 10;
	
	if (debug == SCENE)
	{
		//cout << "show SCENE" << endl;
		_hdr->use();
		glActiveTexture(GL_TEXTURE0);
		_hdr->uniform("screenTexture", (GLuint)0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		return;
	}
	else if (debug == BRIGHT_ONLY)
	{
		//cout << "show BRIGHT_ONLY" << endl;
		_hdr->use();
		glActiveTexture(GL_TEXTURE0);
		_hdr->uniform("screenTexture", (GLuint)0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		return;
	}


	// 1- Blur image with gaussaian blur : 2 framebuffer used to succesvilevy blur horizontaly and vertically <amount> times
	_blur->use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[horizontal]);
		_blur->uniform("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		_blur->uniform("image", (GLuint)0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : blurColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if (debug == BLURRED_BRIGHT)
	{
		//cout << "show BLURRED_BRIGHT" << endl;
		_hdr->use();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		_hdr->uniform("screenTexture", (GLuint)0);
		glBindTexture(GL_TEXTURE_2D, blurColorbuffers[!horizontal]);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		return;
	}


	
	_bloom->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	_bloom->uniform("scene", (GLuint)0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	_bloom->uniform("bloomBlur", (GLuint)1);
	glBindTexture(GL_TEXTURE_2D, blurColorbuffers[!horizontal]);
	_bloom->uniform("exposure", exposure);

	glBindVertexArray(screenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

GLuint BloomEffect::frameBuffer() const { return hdrFBO; }

void BloomEffect::setupGaussianBlurFBOs(int screenWidth, int screenHeight)
{
	glGenFramebuffers(2, blurFBOs);
	glGenTextures(2, blurColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[i]);
		glBindTexture(GL_TEXTURE_2D, blurColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Gaussian Framebuffer n° "<< i << " not complete!" << std::endl;
	}
}
