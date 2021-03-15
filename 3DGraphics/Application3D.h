#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Input.h"
class Application3D : public aie::Application 
{
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();
	
	virtual void update(float deltaTime);
	virtual void draw();
	void doWorldFixedUpdate(float timeStep);
	void doWorldRenderUpdate(float timeStep);
	void pauseWorld();
	void unPauseWorld();

protected:
	aie::Input* input = nullptr;
	class GuiHud* guiHud = nullptr;
	class FlyCamera* cam = nullptr;
	bool hasDoneRenderUpdate = false;
	bool paused = false;
	float percentageToNextTick = 1.0F;
	float timeStep = 0.0333333F;
};