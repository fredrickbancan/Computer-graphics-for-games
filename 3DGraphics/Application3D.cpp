#include "Application3D.h"
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glfw/glfw3.h>
#include <iostream>
#include "MouseHelper.h"
#include "FlyCamera.h"
#include "GuiHud.h"
#include "Input.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using aie::Input;

Application3D::Application3D() 
{
	
}

Application3D::~Application3D() 
{
	MouseHelper::close();
	delete cam;
}

bool Application3D::startup() 
{
	MouseHelper::init();

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	cam = new FlyCamera(glm::vec3(0, 5, -10));

	setBackgroundColour(0.25f, 0.55f, 0.75f);

	setShowCursor(false);
	glfwSetCursorPos(getWindowPtr(), getWindowWidth() / 2, getWindowHeight() / 2);
	setVSync(false);
	guiHud = new GuiHud(this);
	input = Input::getInstance();
	return true;
}

void Application3D::shutdown() 
{
	delete guiHud;
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;
	while (accumulatedTime >= timeStep)
	{
		accumulatedTime -= timeStep;
		doWorldFixedUpdate(timeStep);

		if (!hasDoneRenderUpdate)//do a render update once in this loop. Multiple render updates within the loop is inifficent and unecessary. Render updates do not need to be done every frame for most game objects.
		{
			doWorldRenderUpdate(timeStep);
			hasDoneRenderUpdate = true;
		}
	}

	//this is useful for interpolating the rendering of physics objects in between physics steps
	percentageToNextTick = accumulatedTime / timeStep;

	hasDoneRenderUpdate = false;
	//TODO: 
	if(!paused)
	MouseHelper::get()->update(getWindowWidth() / 2, getWindowHeight() / 2, input->getMouseX(), input->getMouseY());

	//reset cursor back to center of window
	if(!paused)
	glfwSetCursorPos(getWindowPtr(), getWindowWidth()/2, getWindowHeight() / 2);

	if(!paused)
	cam->onUpdate(percentageToNextTick, deltaTime);

	guiHud->onUpdate(deltaTime);

	// pause if we press escape
	if (input->wasKeyPressed(aie::INPUT_KEY_ESCAPE) || glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == 1 || glfwGetWindowAttrib(m_window, GLFW_FOCUSED) != 1)
	{
		if (paused)unPauseWorld(); else pauseWorld();
	}
}

void Application3D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// draw 3D gizmos
	Gizmos::draw(cam->getProjectionMatrix() * cam->getViewMatrix());

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	guiHud->draw();

}

void Application3D::doWorldFixedUpdate(float timeStep)
{
	if(!paused)
	cam->onWorldFixedUpdate(timeStep);
}

void Application3D::doWorldRenderUpdate(float timeStep)
{
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

	guiHud->onWorldRenderUpdate(timeStep);
}

void Application3D::pauseWorld()
{
	paused = true;
	setShowCursor(true);
}

void Application3D::unPauseWorld()
{
	paused = false;
	setShowCursor(false);
	glfwSetCursorPos(getWindowPtr(), getWindowWidth() / 2, getWindowHeight() / 2);
}
