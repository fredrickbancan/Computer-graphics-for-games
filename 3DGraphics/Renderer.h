#pragma once
#include "Rendering.h"
#include <vector>

namespace aie
{
	class Input;
};

/*Singleton class for miscelanious rendering requests for different render types with different shaders*/
/*Very naieve rendering logic. One draw call per requested object draw. Very inneficient, but simple.*/
class Renderer
{
public:
	static void init() { close(); singletonInstance = new Renderer(); }
	static Renderer* getInstance() { return singletonInstance; }
	static void close() { if (singletonInstance)delete singletonInstance; }

	/*Resizes viewport and relevant fbos*/
	void onWindowResize(int width, int height);

	/*Draws each light in provided light vector as a point*/
	void drawLightsAsPoints(const std::vector<struct PointLight*> lights);

	/*Draws provided textured brush*/
	void drawTexturedBrush(class TexturedBrush* tb);

	/*Draws provided textured surface*/
	void drawTexturedSurface(class TexturedSurface* ts);

	/*Draws provided textured model*/
	void drawTexturedModel(class TexturedModel* tm);

	/*Detects input for debug options such as wireframe and light points*/
	void doDebugInputs(aie::Input* input);

	/*Returns dimentions of render frame*/
	glm::vec2 getRenderFrameSize();

	/*Prepares renderer for rendering, binds offscreen fbo*/
	void begin();

	/*Completes rendering, binds origional fbo*/
	void end();
	Renderer();
	~Renderer();
private:
	void renderFullScreenQuadWithFrameBufTex();
	
	/*Builds bayer matrix dithering texture*/
	void initDitherTexture();

	void initTexturedBrushRendering();
	void initTexturedQuadRendering();
	bool debugWireFrameMode = false;
	bool debugLightMode = false;

	unsigned int ditherTexID = 0;

	unsigned int texBrushVaoID = 0;
	unsigned int texBrushIboID = 0;
	unsigned int texBrushVboID = 0;

	unsigned int texQuadVaoID = 0;
	unsigned int texQuadVboID = 0;

	class Shader* shader_TEXTURED_LIT_FOG_TESS = nullptr;
	class Shader* shader_TEXTURED_LIT_FOG = nullptr;
	class Shader* shader_TEXTURED_LIT_FOG_BILLBOARD = nullptr;
	class Shader* shader_FULLSCREENQUAD = nullptr;

	class FrameBuffer* fullScreenFrameBuffer;

	static Renderer* singletonInstance;
};