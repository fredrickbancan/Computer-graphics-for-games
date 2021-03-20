#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
enum class RenderType : int
{
	NONE,
	TEXTURED_ALBEDO,
	TEXTURED_LIT_FOG,
	TEXTURED_LIT_TRANSPARENT_FOG
};

struct Vertex
{
	Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal, glm::vec4 color) : pos(pos), uv(uv), normal(normal), color(color) {};
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec4 color;

	static constexpr unsigned int sizeInBytes = 12 * sizeof(float);
};

namespace aie
{
	class Input;
};

/*Singleton class for miscelanious rendering requests for different render types with different shaders*/
class Renderer
{
public:
	static void init() { close(); singletonInstance = new Renderer(); }
	static Renderer* getInstance() { return singletonInstance; }
	static void close() { if (singletonInstance)delete singletonInstance; }

	void onWindowResize(int width, int height);
	void drawLightsAsPoints(const std::vector<struct PointLight*> lights);
	void drawTexturedBrush(class TexturedBrush* tb);
	void doDebugInputs(aie::Input* input);
	void begin();
	void end();
	Renderer();
	~Renderer();
private:
	void renderFullScreenQuadWithFrameBufTex();
	void buildDitherTexture();
	void setUpTexturedBrushRendering();
	void setUpFullScreenQuadRendering();
	bool debugWireFrameMode = false;
	bool debugLightMode = false;

	unsigned int ditherTexID = 0;

	unsigned int texBrushVaoID = 0;
	unsigned int texBrushIboID = 0;
	unsigned int texBrushVboID = 0;

	unsigned int fullScreenQuadVaoID = 0;
	unsigned int fullScreenQuadVboID = 0;

	class Shader* shader_NONE = nullptr;
	class Shader* shader_TEXTURED_ALBEDO = nullptr;
	class Shader* shader_TEXTURED_LIT_FOG = nullptr;
	class Shader* shader_TEXTURED_LIT_TRANSPARENT_FOG = nullptr;
	class Shader* shader_FULLSCREENQUAD = nullptr;

	class FrameBuffer* fullScreenBuffer;

	static Renderer* singletonInstance;
};