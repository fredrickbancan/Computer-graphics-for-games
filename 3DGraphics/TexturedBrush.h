#pragma once
#include "glm/vec3.hpp"
#include <string>
namespace aie
{
	class Texture;
}
/*Textured brush is a convex peice of geometry, in this case an AABB, with a texutre and possibly physical material properties.*/
class TexturedBrush
{
public:
	TexturedBrush(float posX, float posY, float posZ, float extentX, float extentY, float extentZ, const std::string& texture, float opacity = 1.0F);
	TexturedBrush(glm::vec3 pos, glm::vec3 extents, const std::string& texture, float opacity = 1.0F);
	~TexturedBrush();
	glm::vec3 getPos() { return pos; }
	glm::vec3 getExtents() { return dimentions; }
	const std::string& getTexName() { return texName; }
	int getRenderType() { return renderType; }
	float getOpacity() { return opacity; }
	void bindTexture(int slot);

private:
	glm::vec3 dimentions = glm::vec3{0.5F,0.5F,0.5F};
	glm::vec3 pos = glm::vec3{0,0,0};
	float opacity = 1.0F;
	std::string texName = nullptr;
	aie::Texture* texture = nullptr;
    int renderType;
};