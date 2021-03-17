#pragma once

enum class RenderType : int
{
	NONE = -1,
	TEXTURED_ALBEDO,
	TEXTURED_LIT_FOG,
	TEXTURED_LIT_TRANSPARENT_FOG
};

/*Singleton class for miscelanious rendering requests for different render types with different shaders*/
class Renderer
{
public:
	static void init() { close(); singletonInstance = new Renderer(); }
	static Renderer* getInstance() { return singletonInstance; }
	static void close() { if (singletonInstance)delete singletonInstance; }

	void drawTexturedBrush(class TexturedBrush* tb);

	Renderer();
	~Renderer();
private:
	static Renderer* singletonInstance;
};