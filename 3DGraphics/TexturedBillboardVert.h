#pragma once
#include "TexturedSurface.h"

/*Flat single-sided face which has the y rotation component ignored when rendered resulting in a billboard*/
class TexturedBillboardVert : public TexturedSurface
{
public:
	TexturedBillboardVert(float posX, float posY, float posZ, float extentX, float extentY, float rotX, float rotZ, const std::string& texture, float opacity = 1.0F) : TexturedSurface(posX, posY, posZ, extentX, extentY, rotX, 0, rotZ, texture, opacity) {}
};