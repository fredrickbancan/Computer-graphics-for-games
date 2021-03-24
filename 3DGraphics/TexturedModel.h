#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include <string>
struct Vertex;

namespace aie
{
	class Texture;
}

class TexturedModel
{
public:
	TexturedModel(const std::string& objFileName, const std::string& textureName);
	~TexturedModel();

	Vertex* getModelVertices() const;
	unsigned int* getModelIndices() const;
	void bindVAO();
	void bindTexture();
	void setPos(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void rotate(float x, float y, float z);
	glm::vec3 getPos() { return pos; }
	glm::vec3 getScale() { return scale; }
	glm::mat4 getRotation() { return rotation; }
	int getIndicesCount() { return indicesCount; }
	int getVerticesCount() { return vertexCount; }
	int getRenderType() { return renderType; }
private:
	void initVao();
	Vertex* modelVertices = nullptr;
	int vertexCount = 0;
	unsigned int* modelIndices = nullptr;
	int indicesCount = 0;
	aie::Texture* modelTexture = nullptr;
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ibo = 0;
	glm::mat4 rotation;
	glm::vec3 pos;
	glm::vec3 scale;
	int renderType = 0;
};