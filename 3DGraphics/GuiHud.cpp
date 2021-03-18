#include "GuiHud.h"
#include "Gizmos.h"
#include "Application3D.h"
#include <string>

GuiHud::GuiHud(Application3D* referenceApp) : refApp(referenceApp)
{
	font = new aie::Font("./font/consolas.ttf", 25);
	renderer = new aie::Renderer2D();
}

GuiHud::~GuiHud()
{
	delete font;
	delete renderer;
}

void GuiHud::onWorldRenderUpdate(float timeStep)
{
	// demonstrate 2D gizmos 
	//(crosshair)
	aie::Gizmos::add2DCircle(glm::vec2(refApp->getWindowWidth() / 2, refApp->getWindowHeight() / 2), 3, 16, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(refApp->getWindowWidth() / 2, refApp->getWindowHeight() / 2), 2, 16, glm::vec4(0, 1, 0, 1));
}

void GuiHud::draw()
{
	renderer->begin();
	renderer->setRenderColour(0,1,0,1);
	unsigned int fps = refApp->getFPS();
	renderer->drawText(font, ( std::string("FPS: ") + std::to_string(fps) ).c_str(), 0, refApp->getWindowHeight() - 25);
	renderer->setRenderColour(1,1,1,1);
	renderer->end();

}

void GuiHud::onUpdate(float deltaTime)
{

}
