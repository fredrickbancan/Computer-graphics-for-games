#pragma once
#include "Font.h"
#include "Renderer2D.h"
/*Class for containing all gui and hud related objects and logic for the application*/
class GuiHud
{
public: 
	GuiHud(class Application3D* referenceApp);
	~GuiHud();

	/*Called every frame, updates gui/hud components and input.*/
	void onUpdate(float deltaTime);

	void onWorldRenderUpdate(float timeStep);

	void draw();

private:
	aie::Renderer2D* renderer = nullptr; 
	aie::Font* font = nullptr;
	class Application3D* refApp = nullptr;
};