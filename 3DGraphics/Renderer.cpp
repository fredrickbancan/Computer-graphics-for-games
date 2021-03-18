#include "Renderer.h"
#include "gl_core_4_4.h"
#include "TexturedBrush.h"
#include "Texture.h"
#include <string>
#include "Gizmos.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include "Shaders.h"
#include "Application3D.h"
using namespace aie;

Renderer* Renderer::singletonInstance = nullptr;

// cube vertices
   //
   //               9---------8
   //              /         /
   //             /         /
   //            10-------11
   //
   //    13         20-------21         4       front: {  0,  1,  2 }, {  0,  2,  3 }
   //    /|         |         |        /|       right: {  4,  5,  6 }, {  4,  6,  7 }
   //   / |         |         |       / |         top: {  8,  9, 10 }, {  8, 10, 11 }
   //  12 |      1---------0  |      5  |        left: { 12, 13, 14 }, { 12, 14, 15 }
   //  |  |      |  |      |  |      |  |      bottom: { 16, 17, 18 }, { 16, 18, 19 }
   //  | 14      |  23-----|-22      |  7        back: { 20, 21, 22 }, { 20, 22, 23 }
   //  | /       |         |         | /
   //  |/        |         |         |/
   //  15        2---------3         6
   //
   //               18-------19
   //              /         /
   //             /         /
   //            17-------16
   //

static unsigned int cubeIndices[] =
{ 
	0, 1, 2, 0, 2, 3,//back
    4, 5, 6, 4, 6, 7,//right
    8,  9, 10, 8, 10, 11,//top
   12, 13, 14, 12, 14, 15,//left
   16, 17, 18, 16, 18, 19,//bottom
   20, 21, 22, 20, 22, 23//front
};

static Vertex cubeVertices[] =
{
	//back face
	Vertex({0.5F, 0.5F, -0.5F}, {1,0}, {0,0,-1}, {1,1,1,1}),
	Vertex({-0.5F, 0.5F, -0.5F}, {0,0}, {0,0,-1}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, -0.5F}, {0,1}, {0,0,-1}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, -0.5F}, {1,1}, {0,0,-1}, {1,1,1,1}),

	//right face
	Vertex({0.5F, 0.5F, 0.5F}, {1,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, -0.5F}, {0,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, -0.5F}, {0,1}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.5F}, {1,1}, {0,0,1}, {1,1,1,1}),

	//top face
	Vertex({0.5F, 0.5F, 0.5F}, {1,0}, {0,1,0}, {1,1,1,1}),
	Vertex({-0.5F, 0.5F, 0.5F}, {0,0}, {0,1,0}, {1,1,1,1}),
	Vertex({-0.5F, 0.5F, -0.5F}, {0,1}, {0,1,0}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, -0.5F}, {1,1}, {0,1,0}, {1,1,1,1}),

	//left face
	Vertex({-0.5F, 0.5F, -0.5F}, {1,0}, {-1,0,0}, {1,1,1,1}),
	Vertex({-0.5F, 0.5F, 0.5F}, {0,0}, {-1,0,0}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, 0.5F}, {0,1}, {-1,0,0}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, -0.5F}, {1,1}, {-1,0,0}, {1,1,1,1}),

	//bottom face
	Vertex({0.5F, -0.5F, -0.5F}, {1,0}, {0,-1,0}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, -0.5F}, {0,0}, {0,-1,0}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, 0.5F}, {0,1}, {0,-1,0}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.5F}, {1,1}, {0,-1,0}, {1,1,1,1}),

	//front face
	Vertex({-0.5F, 0.5F, 0.5F}, {1,0}, {1,0,0}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, 0.5F}, {0,0}, {1,0,0}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.5F}, {0,1}, {1,0,0}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, 0.5F}, {1,1}, {1,0,0}, {1,1,1,1})
};

Renderer::Renderer()
{
	shader_NONE = nullptr;
	shader_TEXTURED_ALBEDO = new Shader("shaders/TEXTURED_ALBEDO.shader");
	shader_TEXTURED_LIT_FOG = new Shader("shaders/TEXTURED_LIT_FOG.shader");
	shader_TEXTURED_LIT_TRANSPARENT_FOG = new Shader("shaders/TEXTURED_LIT_TRANSPARENT_FOG.shader");


	//making vao
	glGenVertexArrays(1, &texBrushVaoID);
	glBindVertexArray(texBrushVaoID);

	//making index buffer
	glGenBuffers(1, &texBrushIboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texBrushIboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), cubeIndices, GL_STATIC_DRAW);

	//making vertex buffer
	glGenBuffers(1, &texBrushVboID);
	glBindBuffer(GL_ARRAY_BUFFER, texBrushVboID);
	glBufferData(GL_ARRAY_BUFFER, Vertex::sizeInBytes * 24, cubeVertices, GL_STATIC_DRAW);

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

Renderer::~Renderer()
{
	if (shader_TEXTURED_ALBEDO) delete shader_TEXTURED_ALBEDO;
	if (shader_TEXTURED_LIT_FOG) delete shader_TEXTURED_LIT_FOG;
	if (shader_TEXTURED_LIT_TRANSPARENT_FOG) delete shader_TEXTURED_LIT_TRANSPARENT_FOG;

	//deleting buffers
	glDeleteBuffers(1, &texBrushIboID);//index buffer
	glDeleteBuffers(1, &texBrushVboID);//vbo 
	glDeleteVertexArrays(1, &texBrushVaoID);//vao
}

void Renderer::drawTexturedBrush(TexturedBrush* tb)                  
{
	glDisable(GL_CULL_FACE);
	glBindVertexArray(texBrushVaoID);
	tb->bindTexture(0);
	glm::vec3 pos = tb->getPos();
	glm::vec3 exts = tb->getExtents();
	float alpha = tb->getOpacity();
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, exts * 2.0F);

	switch ((RenderType)tb->getRenderType())
	{
	case RenderType::NONE:
		break;

	case RenderType::TEXTURED_ALBEDO:
		shader_TEXTURED_ALBEDO->bind();
		break;

	case RenderType::TEXTURED_LIT_FOG:
		shader_TEXTURED_LIT_FOG->bind();
		shader_TEXTURED_LIT_FOG->setUniformMat4f("modelMatrix", modelMatrix);
		shader_TEXTURED_LIT_FOG->setUniformMat4f("viewMatrix", Application3D::getInstance()->getViewMatrix());
		shader_TEXTURED_LIT_FOG->setUniformMat4f("projectionMatrix", Application3D::getInstance()->getProjectionMatrix());
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		break;

	case RenderType::TEXTURED_LIT_TRANSPARENT_FOG:
		shader_TEXTURED_LIT_TRANSPARENT_FOG->bind();
		break;

	default:
		break;
	}
}