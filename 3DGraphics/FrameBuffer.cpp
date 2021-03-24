#include "FrameBuffer.h"
#include "gl_core_4_4.h"
#include "glm/vec2.hpp"
#include "Application3D.h"

FrameBuffer::FrameBuffer(int width, int height) : width(width), height(height)
{
    setUpFrameBuffer();
}

FrameBuffer::FrameBuffer(float resolutionFactor) : resolutionFactor(resolutionFactor)
{
    glm::vec2 s = Application3D::getInstance()->getFrameSize();
    s *= resolutionFactor;
    this->width = s.x;
    this->height = s.y;
    relativeResolution = true;
    setUpFrameBuffer();
}

FrameBuffer::~FrameBuffer()
{
    unBindAndBindTexture();
    glDeleteFramebuffers(1, &frameBufferID);
    glDeleteTextures(1, &textureID);
    glDeleteRenderbuffers(1, &depthBufferID);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glm::vec2 s = Application3D::getInstance()->getFrameSize();
    if (relativeResolution)
    {
        s *= resolutionFactor;
    }
    Application3D::getInstance()->onWindowResize(s.x, s.y);
}

void FrameBuffer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::unBindAndBindTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glm::vec2 s = Application3D::getInstance()->getFrameSize();
    Application3D::getInstance()->onWindowResize(s.x, s.y);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void FrameBuffer::resize(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);

    if (relativeResolution)
    {
        this->width = width * resolutionFactor;
        this->height = height * resolutionFactor;
    }

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, this->width, this->height);


    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

glm::vec2 FrameBuffer::getSize()
{
    return glm::vec2(width, height);
}

void FrameBuffer::setUpFrameBuffer()
{
    glGenFramebuffers(1, &frameBufferID);
    glGenTextures(1, &textureID);
    glGenRenderbuffers(1, &depthBufferID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width , height);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
