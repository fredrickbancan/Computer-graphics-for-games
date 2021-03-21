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
#include "Renderer.h"
#include "TexturedBrush.h"
#include "TexturedSurface.h"
#include "PointLight.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using aie::Input;

Application3D* Application3D::singletonInstance = nullptr;

void onWindowResize_Callback(GLFWwindow*, int width, int height)
{
	Application3D::getInstance()->onWindowResize(width, height);
	Renderer::getInstance()->onWindowResize(width, height);
}

bool Application3D::startup() 
{
	std::cout << "starting...." << std::endl;
	std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
	MouseHelper::init();
	Renderer::init();
	Gizmos::create(10000, 10000, 10000, 10000);// initialise gizmo primitive counts
	setBackgroundColour(0.0f, 0.0f, 0.0f);
	glfwSetCursorPos(window, getWindowWidth() / 2, getWindowHeight() / 2);
	glfwSetWindowSizeCallback(window, onWindowResize_Callback);
	setVSync(false);
	glfwSwapInterval(0);
	setShowCursor(false);
	cam = new FlyCamera(glm::vec3(0, 2, 10));
	cam->setAspectRatio((float)getWindowWidth() / (float)getWindowHeight());
	glViewport(0, 0, getWindowWidth(), getWindowHeight());
	guiHud = new GuiHud(this);
	input = Input::getInstance();
	initialized = true;

	int segments = 5;
	float floorSize = 100.0F;
	float halfFloorSize = floorSize * 0.5F;
	float panelSize = floorSize / segments;
	float roomheight = 20;
	for (int x = 0; x < segments; x++)//building floor
	{
		for (int z = 0; z < segments; z++)
		{
			texturedSurfaces.push_back(new TexturedSurface(-halfFloorSize + x * panelSize + (panelSize * 0.5F), 0, -halfFloorSize + z * panelSize + (panelSize * 0.5F), panelSize * 0.5F, panelSize * 0.5F, -90, 0, 0, "trsnow.png"));
		}
		
	}

	for (int x = 0; x < segments; x++)//building ceiling
	{
		for (int z = 0; z < segments; z++)
		{
			texturedSurfaces.push_back(new TexturedSurface(-halfFloorSize + x * panelSize + (panelSize * 0.5F), roomheight, -halfFloorSize + z * panelSize + (panelSize * 0.5F), panelSize * 0.5F, panelSize * 0.5F, 90, 0, 0, "trstone2.png"));
		}

	}

	for (int x = 0; x < segments; x++)//building negZ wall
	{
			texturedSurfaces.push_back(new TexturedSurface(-halfFloorSize + x * panelSize + (panelSize * 0.5F), roomheight * 0.5F, -halfFloorSize , panelSize * 0.5F, roomheight * 0.5F, 0, 0, 0, "tricewall.png"));
	}
	for (int z = 0; z < segments; z++)//building negX wall
	{
		texturedSurfaces.push_back(new TexturedSurface(-halfFloorSize , roomheight * 0.5F, -halfFloorSize + z * panelSize + (panelSize * 0.5F), panelSize * 0.5F, roomheight * 0.5F, 0, 90, 0, "tricewall.png"));
	}
	for (int x = 0; x < segments; x++)//building posZ wall
	{
		texturedSurfaces.push_back(new TexturedSurface(-halfFloorSize + x * panelSize + (panelSize * 0.5F), roomheight * 0.5F, halfFloorSize, panelSize * 0.5F, roomheight * 0.5F, 0, 180, 0, "tricewall.png"));
	}
	for (int z = 0; z < segments; z++)//building posX wall
	{
		texturedSurfaces.push_back(new TexturedSurface(halfFloorSize, roomheight * 0.5F, -halfFloorSize + z * panelSize + (panelSize * 0.5F), panelSize * 0.5F, roomheight * 0.5F, 0, -90, 0, "tricewall.png"));
	}

	float wallSize = 5;
	texturedBrushes.push_back(new TexturedBrush(wallSize*3, wallSize, -10.0F, wallSize, wallSize, wallSize, "trstone.png"));
	texturedBrushes.push_back(new TexturedBrush(wallSize, wallSize, -10.0F, wallSize, wallSize, wallSize, "leafyrocks.png"));
	texturedBrushes.push_back(new TexturedBrush(-wallSize, wallSize, -10.0F, wallSize, wallSize, wallSize, "leafyrocks.png"));
	texturedBrushes.push_back(new TexturedBrush(-wallSize*3, wallSize, -10.0F, wallSize, wallSize, wallSize, "trstone.png"));

	texturedBrushes.push_back(new TexturedBrush(wallSize*3, wallSize, 20.0F, wallSize, wallSize, wallSize, "leafyrocks.png"));
	texturedBrushes.push_back(new TexturedBrush(wallSize, wallSize, 20.0F, wallSize, wallSize, wallSize, "trstone.png"));
	texturedBrushes.push_back(new TexturedBrush(-wallSize, wallSize, 20.0F, wallSize, wallSize, wallSize, "trstone.png"));
	texturedBrushes.push_back(new TexturedBrush(-wallSize*3, wallSize, 20.0F, wallSize, wallSize, wallSize, "leafyrocks.png"));

	texturedBrushes.push_back(new TexturedBrush(0, 2.0F, 0.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));
	texturedBrushes.push_back(new TexturedBrush(8, 2.0F, 4.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));
	texturedBrushes.push_back(new TexturedBrush(-8, 2.0F, 4.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));
	texturedBrushes.push_back(new TexturedBrush(15, 2.0F, -25, 2.0F, 2.0F, 2.0F, "trchimken.png"));

	rotatingTexturedBrushes.push_back(new TexturedBrush(30, 4.0F, -6.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));
	rotatingTexturedBrushes.push_back(new TexturedBrush(30, 4.0F, 0.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));
	rotatingTexturedBrushes.push_back(new TexturedBrush(30, 4.0F, 6.0F, 2.0F, 2.0F, 2.0F, "trchimken.png"));

	pointLights.push_back(new PointLight({ 0,10, 5 }, {1,1,1}, 6, 17, 1.2));//white middle light
	pointLights.push_back(new PointLight({ 15,5, -25}, {0.9F,0.5F,0.2F}, 12, 16, 1.3F));//orange fire light
	pointLights.push_back(new PointLight({ 25,12, 30 }, {0.25F,0.22F,0.15F}, 30, 70, 0.6F));//warmish white corner room light
	pointLights.push_back(new PointLight({ -25,12, -25 }, { 0.152F, 0.211F, 0.468F }, 30, 70, 1.1F));//blue corner room light
	pointLights.push_back(new PointLight({ 33, 1, 0 }, { 0.152F, 0.611F, 0.568F }, 3, 12, 0.9F));//rotating brush area light bottom green ish
	pointLights.push_back(new PointLight({ 31, 7, 0 }, { 0.652F, 0.611F, 0.568F }, 3, 12, 1.1F));//rotating brush area light top gold ish
	return true;
}

void Application3D::shutdown() 
{
	std::cout << "shutting down...." << std::endl;
	Gizmos::destroy();
	MouseHelper::close();
	Renderer::close();
	delete cam;
	delete guiHud;

	for (std::vector<TexturedBrush*>::iterator i = texturedBrushes.begin(); i != texturedBrushes.end(); ++i)
	{
		TexturedBrush* p = *i;
		delete p;
	}
	for (std::vector<TexturedBrush*>::iterator i = rotatingTexturedBrushes.begin(); i != rotatingTexturedBrushes.end(); ++i)
	{
		TexturedBrush* p = *i;
		delete p;
	}

	for (std::vector<TexturedSurface*>::iterator i = texturedSurfaces.begin(); i != texturedSurfaces.end(); ++i)
	{
		TexturedSurface* p = *i;
		delete p;
	}

	texturedBrushes.clear();
}

void Application3D::update(float deltaTime) 
{
	if (glfwWindowShouldClose(window)) quit();
	if (!paused)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		MouseHelper::get()->update(getWindowWidth() / 2, getWindowHeight() / 2, x, y);
		cam->onUpdate(percentageToNextTick, deltaTime);
		glfwSetCursorPos(getWindowPtr(), getWindowWidth()/2, getWindowHeight() / 2);//reset cursor back to center of window
		doWorldFixedUpdateLoop(deltaTime);
	}

	Renderer::getInstance()->doDebugInputs(input);

	guiHud->onUpdate(deltaTime);

	if (input->wasKeyPressed(aie::INPUT_KEY_ESCAPE) || (!paused && glfwGetWindowAttrib(window, GLFW_FOCUSED) == 0))// pause if we press escape or lose window focus
	{
		if (paused) unPauseWorld(); else pauseWorld();
	}
}

void Application3D::draw() 
{
	clearScreen();// wipe the screen to the background colour
	Renderer::getInstance()->begin();
	for (std::vector<TexturedBrush*>::iterator i = texturedBrushes.begin(); i != texturedBrushes.end(); i++)//Doing this intead of using render updates since they are not dynamic objects and this is a lot simpler than setting up batching, render object inheritence etc.
	{
		Renderer::getInstance()->drawTexturedBrush(*i);
	}

	for (std::vector<TexturedBrush*>::iterator i = rotatingTexturedBrushes.begin(); i != rotatingTexturedBrushes.end(); ++i)
	{
		Renderer::getInstance()->drawTexturedBrush(*i);
	}

	for (std::vector<TexturedSurface*>::iterator i = texturedSurfaces.begin(); i != texturedSurfaces.end(); ++i)
	{
		Renderer::getInstance()->drawTexturedSurface(*i);
	}
	Renderer::getInstance()->end();
	Gizmos::draw(cam->getProjectionMatrix() * cam->getViewMatrix());// draw 3D gizmos
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	guiHud->draw();
}

void Application3D::doWorldFixedUpdateLoop(float deltaTime)
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
	hasDoneRenderUpdate = false;
	percentageToNextTick = accumulatedTime / timeStep;//this is useful for interpolating the rendering of physics objects in between physics steps
}

void Application3D::doWorldFixedUpdate(float timeStep)
{
	cam->onWorldFixedUpdate(timeStep);
}

void Application3D::doWorldRenderUpdate(float timeStep)
{
	Gizmos::clear();// wipe the gizmos clean for this frame
	Gizmos::addTransform(mat4(1));// add a transform so that we can see the axis
	guiHud->onWorldRenderUpdate(timeStep);
	Renderer::getInstance()->drawLightsAsPoints(pointLights);

	//rotate all the rotating brushes by 15 degrees in each axis every second
	for (std::vector<TexturedBrush*>::iterator i = rotatingTexturedBrushes.begin(); i != rotatingTexturedBrushes.end(); ++i)
	{
		TexturedBrush* p = *i;
		p->rotate(15.0F * timeStep, glm::vec3(0,1,0));
		p->rotate(15.0F * timeStep, glm::vec3(1,0,0));
		p->rotate(15.0F * timeStep, glm::vec3(0,0,1));
	}
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

void Application3D::onWindowResize(int width, int height)
{ 
	if (!initialized) return;
	if (width <= 0 || height <= 0) return;
	cam->setAspectRatio((float)width/(float)height);
	glViewport(0, 0, width, height);
}

glm::vec2 Application3D::getFrameSize()
{
	int w, h;
	glfwGetFramebufferSize(getWindowPtr(), &w, &h);
	return glm::vec2(w, h);
}

glm::mat4 Application3D::getViewMatrix()
{
	return cam->getViewMatrix();
}

glm::mat4 Application3D::getProjectionMatrix()
{
	return cam->getProjectionMatrix();
}

glm::vec3 Application3D::getCamPos()
{
	return cam->getPosition();
}


