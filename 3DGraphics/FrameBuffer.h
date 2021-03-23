#pragma once
#include "glm/vec2.hpp"
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	FrameBuffer(float resolutionFactor);
	~FrameBuffer();
	void bind();
	void clear();
	void unBindAndBindTexture();
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