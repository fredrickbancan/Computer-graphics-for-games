#include "TexturedBrush.h"
#include "Renderer.h"

TexturedBrush::TexturedBrush(float posX, float posY, float posZ, float extentX, float extentY, float extentZ, const char* texture, float opacity) : texName(texture), opacity(opacity)
{
	renderType = (opacity < 1.0F ? RenderType::TEXTURED_LIT_FOG : (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_TRANSPARENT_FOG));
	pos = glm::vec3(posX, posY, posZ);
	dimentions = glm::vec3(extentX, extentY, extentZ);
}

TexturedBrush::TexturedBrush(glm::vec3 pos, glm::vec3 extents, const char* texture, float opacity) : texName(texture), opacity(opacity), pos(pos), dimentions(extents)
{
	renderType = (opacity < 1.0F ? RenderType::TEXTURED_LIT_FOG : (opacity <= 0.0F ? RenderType::NONE : RenderType::TEXTURED_LIT_TRANSPARENT_FOG));
}

TexturedBrush::~TexturedBrush()
{
	if(texName)
	delete texName;
}
