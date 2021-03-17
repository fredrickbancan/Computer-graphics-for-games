#pragma once
#include "glm/vec3.hpp"

/*Textured brush is a convex peice of geometry, in this case an AABB, with a texutre and possibly physical material properties.*/
class TexturedBrush
{
public:
	TexturedBrush(float posX, float posY, float posZ, float extentX, float extentY, float extentZ, const char* texture, float opacity = 1.0F);
	TexturedBrush(glm::vec3 pos, glm::vec3 extents, const char* texture, float opacity = 1.0F);
	~TexturedBrush();

	const char* getTexName() { return texName; }
private:
	glm::vec3 dimentions = glm::vec3{0.5F,0.5F,0.5F};
	glm::vec3 pos = glm::vec3{0,0,0};
	float opacity = 1.0F;
	const char* texName = nullptr;
	enum RenderType renderType;
};