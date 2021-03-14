#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glfw/glfw3.h>
#include <iostream>
#include "MouseHelper.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() 
{
	cam = new FlyCamera(glm::vec3(0,5,-10));
	MouseHelper::init();
}

Application3D::~Application3D() 
{
	MouseHelper::close();
	delete cam;
}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);
	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);
	setShowCursor(false);
	glfwSetInputMode(getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

void Application3D::shutdown() 
{

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	aie::Input* input = aie::Input::getInstance();

	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;
	while (accumulatedTime >= timeStep)
	{
		accumulatedTime -= timeStep;
		doFixedUpdate(timeStep);
	}

	//this is useful for interpolating the rendering of physics objects in between physics steps
	percentageToNextTick = accumulatedTime / timeStep;

	MouseHelper::get()->update(getWindowWidth() / 2, getWindowHeight() / 2, input->getMouseX(), input->getMouseY());

	//reset cursor back to center of window
	glfwSetCursorPos(getWindowPtr(), getWindowWidth()/2, getWindowHeight()/2);

	cam->onUpdate(percentageToNextTick, deltaTime);

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// demonstrate 2D gizmos
	Gizmos::add2DAABB(glm::vec2(getWindowWidth() / 2, 100),glm::vec2(getWindowWidth() / 2 * (fmod(getTime(), 3.f) / 3), 20),vec4(0, 1, 1, 1));

	// quit if we press escape

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() 
{

	// wipe the screen to the background colour
	clearScreen();

	// draw 3D gizmos
	Gizmos::draw(cam->getProjectionMatrix() * cam->getViewMatrix());

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::doFixedUpdate(float timeStep)
{
	cam->onFixedUpdate(timeStep);
}
