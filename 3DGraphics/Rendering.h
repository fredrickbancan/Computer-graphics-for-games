#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum class RenderType : int
{
	NONE,
	TEXTURED_ALBEDO,
	TEXTURED_LIT_FOG,
	TEXTURED_LIT_FOG_BILLBOARD,
	TEXTURED_LIT_FOG_TRANSPARENT
};

struct Vertex
{
	Vertex() : pos(glm::vec3(0)), uv(glm::vec2(0)), normal({0,1,0}), color({ 1,1,1,1 }) {};
	Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal, glm::vec4 color) : pos(pos), uv(uv), normal(normal), color(color) {};
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec4 color;

	static constexpr unsigned int sizeInBytes = 12 * sizeof(float);
};
