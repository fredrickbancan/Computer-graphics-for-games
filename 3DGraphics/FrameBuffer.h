#pragma once
#include "glm/vec2.hpp"

/*Object for abstraction of a framebuffer to render to*/
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	FrameBuffer(float resolutionFactor);
	~FrameBuffer();

	/*Binds the framebuffer and resizes the viewport to match its resolution.*/
	void bind();

	/*clears FBO color and depth buffers*/
	void clear();

	/*Binds the DEFAULT FBO (0) and binds this FBOS texture output to tex slot 0*/
	void unBindAndBindTexture();

	/*Resizes all buffers in this FBO*/
	void resize(int width, int height);
 
	glm::vec2 getSize();
	float getResolutionFactor(){return resolutionFactor;}
protected:
	void setUpFrameBuffer();
	float resolutionFactor = 1.0F;
	bool relativeResolution = false;
	unsigned int frameBufferID;
	unsigned int textureID;
	unsigned int depthBufferID;
	unsigned int width;
	unsigned int height;
};