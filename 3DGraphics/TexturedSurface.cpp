#include "TexturedSurface.h"
#include "Rendering.h"
#include "Texture.h"
#include "gl_core_4_4.h"
#include "glm/ext.hpp"

using namespace aie;

TexturedSurface::TexturedSurface(float posX, float posY, float posZ, float extentX, float extentY, float rotX, float rotY, float rotZ, const std::string& texture, bool billBoard, float opacity) : texName(texture), opacity(opacity)
{
	if (billBoard)
	{
		renderType = (int)RenderType::TEXTURED_LIT_FOG_BILLBOARD;
	}
	else
	{
		renderType = (int)(opacity < 1.0F ? (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_FOG_TRANSPARENT) : RenderType::TEXTURED_LIT_FOG);
	}
	pos = glm::vec3(posX, posY, posZ);
	dimentions = glm::vec2(extentX, extentY);


	//Rotations on billboard surfaces are still important for manipulating the normal to change lighting on the texture
	rotation = glm::mat4(1);
	rotate(rotX, glm::vec3(1,0,0));
	rotate(rotY, glm::vec3(0,1,0));
	rotate(rotZ, glm::vec3(0,0,1));
	this->texture = new Texture((std::string("textures/") + texture).c_str());
	this->texture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

TexturedSurface::TexturedSurface(glm::vec3 pos, glm::vec2 extents, float rotationAngle, glm::vec3 rotationAxis, const std::string& texture, bool billBoard, float opacity) : texName(texture), opacity(opacity), pos(pos), dimentions(extents)
{
	if (billBoard)
	{
		renderType = (int)RenderType::TEXTURED_LIT_FOG_BILLBOARD;
	}
	else
	{
		renderType = (int)(opacity < 1.0F ? (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_FOG_TRANSPARENT) : RenderType::TEXTURED_LIT_FOG);
	}

	rotation = glm::mat4(1);
	setRotation(rotationAngle, rotationAxis);
	this->texture = new Texture((std::string("textures/") + texture).c_str());
	this->texture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
TexturedSurface::~TexturedSurface()
{
	delete texture;
}

void TexturedSurface::bindTexture(int slot)
{
	this->texture->bind(slot);
}

void TexturedSurface::setRotation(float degrees, glm::vec3 axis)
{
	rotation = glm::rotate(glm::mat4(1), glm::radians(degrees), axis);
}

void TexturedSurface::rotate(float degrees, glm::vec3 axis)
{
	rotation = glm::rotate(rotation, glm::radians(degrees), axis);
}
