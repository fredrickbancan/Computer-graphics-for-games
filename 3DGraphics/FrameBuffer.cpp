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
    width = (int)s.x;
    height = (int)s.y;
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
    if (relativeResolution)
    {
        glm::vec2 s = Application3D::getInstance()->getFrameSize();
        Application3D::getInstance()->onWindowResize(s.x * resolutionFactor, s.y * resolutionFactor);
    }
    else
    {

		Application3D::getInstance()->onWindowResize(width, height);
    }
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width * resolutionFactor, height * resolutionFactor, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width * resolutionFactor, height * resolutionFactor);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width , height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width * resolutionFactor, height * resolutionFactor, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width * resolutionFactor, height * resolutionFactor);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
