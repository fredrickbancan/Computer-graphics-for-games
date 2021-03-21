#include "TexturedBrush.h"
#include "Renderer.h"
#include "Texture.h"
#include "gl_core_4_4.h"
#include "glm/ext.hpp"
using namespace aie;
TexturedBrush::TexturedBrush(float posX, float posY, float posZ, float extentX, float extentY, float extentZ, const std::string& texture, float opacity) : texName(texture), opacity(opacity)
{
	renderType = (int)(opacity < 1.0F ? (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_TRANSPARENT_FOG) : RenderType::TEXTURED_LIT_FOG);
	pos = glm::vec3(posX, posY, posZ);
	dimentions = glm::vec3(extentX, extentY, extentZ);
	rotation = glm::mat4(1);
	this->texture = new Texture((std::string("textures/") + texture).c_str());
	this->texture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

TexturedBrush::TexturedBrush(glm::vec3 pos, glm::vec3 extents, const std::string& texture, float opacity) : texName(texture), opacity(opacity), pos(pos), dimentions(extents)
{
	renderType = (int) (opacity < 1.0F ? (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_TRANSPARENT_FOG) : RenderType::TEXTURED_LIT_FOG);
	rotation = glm::mat4(1);
	this->texture = new Texture((std::string("textures/") + texture).c_str());
	this->texture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

TexturedBrush::~TexturedBrush()
{
	delete texture;
}

void TexturedBrush::bindTexture(int slot)
{
	this->texture->bind(slot);
}

void TexturedBrush::rotate(float degrees, glm::vec3 axis)
{
	rotation = glm::rotate(rotation, glm::radians(degrees), axis);
}
