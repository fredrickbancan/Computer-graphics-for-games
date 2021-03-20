#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
struct PointLight
{
	PointLight(glm::vec3 pos, glm::vec3 color, float fallOffStart, float fallOffEnd, float luminance) : pos(pos), color(color), fallOffStart(fallOffStart), luminance(luminance) {};
	glm::vec3 pos; 
	glm::vec3 color; 
	float fallOffStart = 1.0F;
	float fallOffEnd = 10.0F;
	float luminance = 1.0F;

	static constexpr int sizeInBytes = 9 * sizeof(float);

	glm::mat4 getMatrix() { return glm::mat4(pos.x, pos.y, pos.z, 0, color.r, color.g, color.b, 0, fallOffStart, fallOffEnd, luminance, 0, 0, 0, 0, 0); }
};

/*

Point light matrix layout:

pos.x			color.r			fallOffStart		0


pos.y			color.g			fallOffEnd			0


pos.z			color.b			luminance			0


0				0				0					0

*/