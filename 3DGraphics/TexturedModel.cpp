#include "gl_core_4_4.h"
#include "TexturedModel.h"
#include "Texture.h"
#include "ObjLoader.h"
#include "Rendering.h"
#include "glm/ext.hpp"

TexturedModel::TexturedModel(const std::string& objFileName, const std::string& textureName) : scale({ 1,1,1 }), pos({0,0,0})
{
	ObjLoader::getModelDataFromObjFile(objFileName, modelVertices, modelIndices, &vertexCount, &indicesCount);
	modelTexture = new aie::Texture((std::string("textures/") + textureName).c_str());
	modelTexture->bind(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	rotation = glm::mat4(1);
	renderType = (int)RenderType::TEXTURED_LIT_FOG;
	initVao();
}

TexturedModel::~TexturedModel()
{
	if(modelVertices)
		delete[] modelVertices;
	if (modelTexture)
		delete modelTexture;
	if (modelIndices)
		delete[] modelIndices;

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}

Vertex* TexturedModel::getModelVertices() const
{
	return modelVertices;
}

unsigned int* TexturedModel::getModelIndices() const
{
	return modelIndices;
}

void TexturedModel::bindVAO()
{
	glBindVertexArray(vao);
}

void TexturedModel::bindTexture()
{
	modelTexture->bind(0);
}

void TexturedModel::setPos(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void TexturedModel::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void TexturedModel::setRotation(float x, float y, float z)
{
	rotation = glm::mat4(1);
	rotate(x, y, z);
}

void TexturedModel::rotate(float x, float y, float z)
{
	rotation = glm::rotate(rotation, glm::radians(x), glm::vec3(1,0,0));
	rotation = glm::rotate(rotation, glm::radians(y), glm::vec3(0,1,0));
	rotation = glm::rotate(rotation, glm::radians(z), glm::vec3(0,0,1));
}

void TexturedModel::initVao()
{
	//making vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//making index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), modelIndices, GL_STATIC_DRAW);

	//making vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * Vertex::sizeInBytes, modelVertices, GL_STATIC_DRAW);

	//configuring layout for vbo
	//pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, Vertex::sizeInBytes, (const void*)0);

	//uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, Vertex::sizeInBytes, (const void*)(3 * sizeof(float)));

	//normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, Vertex::sizeInBytes, (const void*)(5 * sizeof(float)));

	//color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, false, Vertex::sizeInBytes, (const void*)(8 * sizeof(float)));

}
