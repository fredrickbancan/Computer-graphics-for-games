#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "FlyCamera.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	void doFixedUpdate(float timeStep);

protected:

	FlyCamera* cam;

	float percentageToNextTick = 1.0F;
	float timeStep = 0.01F;
};