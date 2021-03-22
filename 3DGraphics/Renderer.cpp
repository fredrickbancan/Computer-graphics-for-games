#include "Renderer.h"
#include "gl_core_4_4.h"
#include "TexturedBrush.h"
#include "TexturedSurface.h"
#include "Texture.h"
#include <string>
#include "Gizmos.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include "Shaders.h"
#include "Application3D.h"
#include "Input.h"
#include "PointLight.h"
#include "FrameBuffer.h"
using namespace aie;

constexpr int maxLights = 8;
//TODO: clean up duplicate code
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


//for triangles
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
	Vertex({0.5F, 0.5F, 0.5F}, {1,0}, {1,0,0}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, -0.5F}, {0,0}, {1,0,0}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, -0.5F}, {0,1}, {1,0,0}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.5F}, {1,1}, {1,0,0}, {1,1,1,1}),

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
	Vertex({-0.5F, 0.5F, 0.5F}, {1,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, 0.5F}, {0,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.5F}, {0,1}, {0,0,1}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, 0.5F}, {1,1}, {0,0,1}, {1,1,1,1})
};

static Vertex quadVertices[] =
{
	//front face
	Vertex({-0.5F, 0.5F, 0.0F}, {1,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, 0.5F, 0.0F}, {0,0}, {0,0,1}, {1,1,1,1}),
	Vertex({0.5F, -0.5F, 0.0F}, {0,1}, {0,0,1}, {1,1,1,1}),
	Vertex({-0.5F, -0.5F, 0.0F}, {1,1}, {0,0,1}, {1,1,1,1})
};

static unsigned char ditherPattern[] = {
			 0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
			48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
			12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
			60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
			 3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
			51, 19, 59, 27, 49, 17, 57, 25,
			 15, 47,  7, 39, 13, 45,  5, 37,
			63, 31, 55, 23, 61, 29, 53, 21 };

Renderer::Renderer()
{
	int maxVertUniformComps = 0; glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertUniformComps);
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "OpenGL Max Vertex Shader Uniform Components: " << maxVertUniformComps << std::endl;

	shader_NONE = nullptr;
	shader_TEXTURED_ALBEDO = new Shader("shaders/TEXTURED_ALBEDO.shader");
	shader_TEXTURED_LIT_FOG = new Shader("shaders/TEXTURED_LIT_FOG.shader", "shaders/PROXIMITY_TESSELLATION.shader");
	shader_TEXTURED_LIT_TRANSPARENT_FOG = new Shader("shaders/TEXTURED_LIT_TRANSPARENT_FOG.shader");
	shader_TEXTURED_LIT_FOG->bind();
	shader_TEXTURED_LIT_FOG->setUniform1i("uTexture", 0);
	shader_TEXTURED_LIT_FOG->setUniform1i("ditherTexture", 1);
	shader_FULLSCREENQUAD = new Shader("shaders/FULLSCREENQUAD.shader");
	setUpTexturedBrushRendering();
	setUpFullScreenQuadRendering();
	glPointSize(10);
}

Renderer::~Renderer()
{
	if (shader_TEXTURED_ALBEDO) delete shader_TEXTURED_ALBEDO;
	if (shader_TEXTURED_LIT_FOG) delete shader_TEXTURED_LIT_FOG;
	if (shader_TEXTURED_LIT_TRANSPARENT_FOG) delete shader_TEXTURED_LIT_TRANSPARENT_FOG;
	if (shader_FULLSCREENQUAD) delete shader_FULLSCREENQUAD;

	//deleting buffers
	glDeleteBuffers(1, &texBrushIboID);//index buffer
	glDeleteBuffers(1, &texBrushVboID);//vbo 
	glDeleteVertexArrays(1, &texBrushVaoID);//vao

	glDeleteTextures(1, &ditherTexID);

	delete fullScreenBuffer;
}

void Renderer::renderFullScreenQuadWithFrameBufTex()
{
	glBindVertexArray(texBrushVaoID);
	shader_FULLSCREENQUAD->bind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ditherTexID);
	glDepthMask(false);
	glDrawArrays(GL_QUADS, 0, 4);
	glDepthMask(true);
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::buildDitherTexture()
{
	glGenTextures(1, &ditherTexID);
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, ditherTexID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, ditherPattern);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::setUpTexturedBrushRendering()
{
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

	//for teselation (setting to quads)
	glPatchParameteri(GL_PATCH_VERTICES, 4);
}

void Renderer::setUpFullScreenQuadRendering()
{
	fullScreenBuffer = new FrameBuffer(0.33333F);

	//making vao
	glGenVertexArrays(1, &texQuadVaoID);
	glBindVertexArray(texQuadVaoID);

	//making vertex buffer
	glGenBuffers(1, &texQuadVboID);
	glBindBuffer(GL_ARRAY_BUFFER, texQuadVboID);
	glBufferData(GL_ARRAY_BUFFER, Vertex::sizeInBytes * 4, quadVertices, GL_STATIC_DRAW);

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
	buildDitherTexture();
}

void Renderer::onWindowResize(int width, int height)
{
	fullScreenBuffer->resize(width, height);
}

void Renderer::drawLightsAsPoints(const std::vector<struct PointLight*> lights)
{
	
	for (std::vector<PointLight*>::const_iterator i = lights.begin(); i != lights.end(); ++i)
	{
		PointLight* p = *i;
		glm::mat4 transform = glm::mat4(1);
		transform = glm::inverse(glm::lookAt(glm::vec3(0), p->pos - Application3D::getInstance()->getCamPos(), glm::vec3(0, 1, 0)));
		transform = glm::rotate(transform, 90.0F, glm::vec3(1,0,0));
		Gizmos::addDisk(p->pos, 0.075, 6, glm::vec4(p->color, 1), &transform);

		if (!debugLightMode)continue;
		Gizmos::addSphere(p->pos, p->fallOffStart, 16, 16, glm::vec4(0,0,0,0));
		Gizmos::addSphere(p->pos, p->fallOffEnd, 16, 16, glm::vec4(0,0,0,0));
	}
}

void Renderer::drawTexturedBrush(TexturedBrush* tb)
{
	glm::mat4 pointLightMats[maxLights]{glm::mat4(0)};

	std::vector<PointLight*> pLights = Application3D::getInstance()->getPointLights();

	int iter = 0;
	for (std::vector<PointLight*>::iterator i = pLights.begin(); i != pLights.end(); ++i)
	{
		pointLightMats[iter] = (*i)->getMatrix();
		if (++iter >= maxLights) break;
	}

	if(debugWireFrameMode)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	glBindVertexArray(texBrushVaoID);
	tb->bindTexture(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ditherTexID);
	glm::vec3 pos = tb->getPos();
	glm::vec3 exts = tb->getExtents();
	float alpha = tb->getOpacity();
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix *= tb->getRotationMatrix();
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
		shader_TEXTURED_LIT_FOG->setUniformMat4fArray("pointLights", iter, &pointLightMats[0][0][0]);
		shader_TEXTURED_LIT_FOG->setUniform1i("activeLights", iter);
		shader_TEXTURED_LIT_FOG->setUniformMat4f("modelMatrix", modelMatrix);
		shader_TEXTURED_LIT_FOG->setUniformMat3f("normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
		shader_TEXTURED_LIT_FOG->setUniformMat4f("viewMatrix", Application3D::getInstance()->getViewMatrix());
		shader_TEXTURED_LIT_FOG->setUniformMat4f("projectionMatrix", Application3D::getInstance()->getProjectionMatrix());
		shader_TEXTURED_LIT_FOG->setUniform3f("camWorldPos", Application3D::getInstance()->getCamPos());
		shader_TEXTURED_LIT_FOG->setUniform1f("fogStart", 50);
		shader_TEXTURED_LIT_FOG->setUniform1f("fogEnd", 100);
		shader_TEXTURED_LIT_FOG->setUniform1f("detailDist", 50);
		shader_TEXTURED_LIT_FOG->setUniform1f("positionResolution", 128);
		shader_TEXTURED_LIT_FOG->setUniform1f("innacuracyOverDistanceFactor", 1.0F);
		glDrawArrays(GL_PATCHES, 0, 24);
		break;

	case RenderType::TEXTURED_LIT_TRANSPARENT_FOG:
		shader_TEXTURED_LIT_TRANSPARENT_FOG->bind();
		break;

	default:
		break;
	}
	if (debugWireFrameMode)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
}

void Renderer::drawTexturedSurface(TexturedSurface* ts)
{
	glm::mat4 pointLightMats[maxLights]{ glm::mat4(0) };

	std::vector<PointLight*> pLights = Application3D::getInstance()->getPointLights();

	int iter = 0;
	for (std::vector<PointLight*>::iterator i = pLights.begin(); i != pLights.end(); ++i)
	{
		pointLightMats[iter] = (*i)->getMatrix();
		if (++iter >= maxLights) break;
	}
	if (debugWireFrameMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glBindVertexArray(texQuadVaoID);
	ts->bindTexture(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ditherTexID);
	glm::vec3 pos = ts->getPos();
	glm::vec2 exts = ts->getExtents();
	float alpha = ts->getOpacity();
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix *= ts->getRotationMatrix();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(exts.x * 2.0F, exts.y * 2.0F, 1));

	glDisable(GL_CULL_FACE);
	switch ((RenderType)ts->getRenderType())
	{

	case RenderType::NONE:
		break;

	case RenderType::TEXTURED_ALBEDO:
		shader_TEXTURED_ALBEDO->bind();
		break;

	case RenderType::TEXTURED_LIT_FOG:
		shader_TEXTURED_LIT_FOG->bind();
		shader_TEXTURED_LIT_FOG->setUniformMat4fArray("pointLights", iter, &pointLightMats[0][0][0]);
		shader_TEXTURED_LIT_FOG->setUniform1i("activeLights", iter);
		shader_TEXTURED_LIT_FOG->setUniformMat4f("modelMatrix", modelMatrix);
		shader_TEXTURED_LIT_FOG->setUniformMat3f("normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix))));
		shader_TEXTURED_LIT_FOG->setUniformMat4f("viewMatrix", Application3D::getInstance()->getViewMatrix());
		shader_TEXTURED_LIT_FOG->setUniformMat4f("projectionMatrix", Application3D::getInstance()->getProjectionMatrix());
		shader_TEXTURED_LIT_FOG->setUniform3f("camWorldPos", Application3D::getInstance()->getCamPos());
		shader_TEXTURED_LIT_FOG->setUniform1f("fogStart", 50);
		shader_TEXTURED_LIT_FOG->setUniform1f("fogEnd", 100);
		shader_TEXTURED_LIT_FOG->setUniform1f("detailDist", 50);
		shader_TEXTURED_LIT_FOG->setUniform1f("positionResolution", 128);
		shader_TEXTURED_LIT_FOG->setUniform1f("innacuracyOverDistanceFactor", 1.0F);
		glDrawArrays(GL_PATCHES, 0, 4);
		break;

	case RenderType::TEXTURED_LIT_TRANSPARENT_FOG:
		shader_TEXTURED_LIT_TRANSPARENT_FOG->bind();
		break;

	default:
		break;
	}
	if (debugWireFrameMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_CULL_FACE);
}

void Renderer::doDebugInputs(Input* input)
{
	if (input->wasKeyPressed(INPUT_KEY_T)) debugWireFrameMode = !debugWireFrameMode;
	if (input->wasKeyPressed(INPUT_KEY_L)) debugLightMode = !debugLightMode;
}

void Renderer::begin()
{
	fullScreenBuffer->bind();
	fullScreenBuffer->clear();
}

void Renderer::end()
{
	fullScreenBuffer->unBindAndBindTexture();
	renderFullScreenQuadWithFrameBufTex();
}
