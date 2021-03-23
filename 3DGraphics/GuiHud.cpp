#include "GuiHud.h"
#include "Gizmos.h"
#include "Application3D.h"
#include <string>

GuiHud::GuiHud(Application3D* referenceApp) : refApp(referenceApp)
{
	font = new aie::Font("./font/consolas.ttf", 20);
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
//	aie::Gizmos::add2DCircle(glm::vec2(refApp->getWindowWidth() / 2, refApp->getWindowHeight() / 2), 3, 16, glm::vec4(0, 0, 0, 1));
	//aie::Gizmos::add2DCircle(glm::vec2(refApp->getWindowWidth() / 2, refApp->getWindowHeight() / 2), 2, 16, glm::vec4(0, 1, 0, 1));
}

void GuiHud::draw()
{
	using namespace std;
	glm::vec3 cp = Application3D::getInstance()->getCamPos();
	glm::vec2 res = Application3D::getInstance()->getRenderFrameSize();
	renderer->begin();
	renderer->setRenderColour(1,1,1,0.5F);
	renderer->drawText(font,  "w,a,s,d,space,ctrl,mouse to move", 0, refApp->getWindowHeight() - 25);
	renderer->drawText(font,  "T to show tesselation/wireframe", 0, refApp->getWindowHeight() - 50);
	renderer->drawText(font,  "L to show lights radius", 0, refApp->getWindowHeight() - 75);
	renderer->drawText(font, "Cam Pos: ", 0, refApp->getWindowHeight() - 100);
	renderer->drawText(font, to_string(cp.x).c_str() , 0, refApp->getWindowHeight() - 125);
	renderer->drawText(font, to_string(cp.y).c_str() , 0, refApp->getWindowHeight() - 150);
	renderer->drawText(font, to_string(cp.z).c_str() , 0, refApp->getWindowHeight() - 175);
	renderer->drawText(font, "Resolution: ", 0, refApp->getWindowHeight() - 200);
	renderer->drawText(font, to_string(res.x).c_str() , 0, refApp->getWindowHeight() - 225);
	renderer->drawText(font, to_string(res.y).c_str() , 0, refApp->getWindowHeight() - 250);
	renderer->setRenderColour(1,1,1,1);
	renderer->end();

}

void GuiHud::onUpdate(float deltaTime)
{

}
