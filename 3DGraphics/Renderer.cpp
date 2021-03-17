#include "Renderer.h"
#include "TexturedBrush.h"
#include "Texture.h"

using namespace aie;

Renderer* Renderer::singletonInstance = nullptr;


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::drawTexturedBrush(TexturedBrush* tb)
{
	Texture tex = Texture(tb->getTexName());//TODO: set up texture dir
}
