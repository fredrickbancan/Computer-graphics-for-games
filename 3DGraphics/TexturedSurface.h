#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <string>
namespace aie
{
	class Texture;
}
/*A double-sided (in rendering) flat textured quadlirateral face*/
class TexturedSurface
{
public:
	TexturedSurface(float posX, float posY, float posZ, float extentX, float extentY, float rotX, float rotY, float rotZ, const std::string& texture, bool billBoard = false, float opacity = 1.0F);
	TexturedSurface(glm::vec3 pos, glm::vec2 extents, float rotationAngle, glm::vec3 rotationAxis, const std::string& texture, bool billBoard = false, float opacity = 1.0F);
	~TexturedSurface();
	glm::vec3 getPos() { return pos; }
	glm::vec2 getExtents() { return dimentions; }
	const std::string& getTexName() { return texName; }
	int getRenderType() { return renderType; }
	float getOpacity() { return opacity; }
	void bindTexture(int slot);
	void setRotation(float degrees, glm::vec3 axis);
	void rotate(float degrees, glm::vec3 axis);
	glm::mat4 getRotationMatrix() { return rotation; };
private:
	float opacity = 1.0F;
	glm::vec2 dimentions = glm::vec2{ 0.5F,0.5F };
	glm::vec3 pos = glm::vec3{ 0,0,0 };
	glm::mat4 rotation;
	std::string texName = nullptr;
	aie::Texture* texture = nullptr;
	int renderType;
};