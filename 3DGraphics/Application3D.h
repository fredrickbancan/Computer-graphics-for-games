#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Input.h"
#include <vector>
/*static class for containing the base logic for the application*/
class Application3D : public aie::Application 
{
public:
	static void start() { deleteInstance(); singletonInstance = new Application3D(); singletonInstance->run("Bootstrap 3D Graphics", 640, 480, false); }
	static Application3D* getInstance() { return singletonInstance; }
	static void deleteInstance() { if (singletonInstance) { delete singletonInstance; } }

	/*Initialize and build game environment*/
	virtual bool startup();

	/*Manage memory and shut down application*/
	virtual void shutdown();

	/*Update input and game objects each frame*/
	virtual void update(float deltaTime);

	/*Render scene*/
	virtual void draw();

	/*Does work updates in succession if needed to catch up with current time. Unless not enough time has passed.*/
	void doWorldFixedUpdateLoop(float deltaTime);

	/*Update game logic which only needs to be updated with a timestep such as render requests and physics*/
	void doWorldFixedUpdate(float timeStep);

	/*Request draw calls for gameobjects on each fixed update instead of each frame. Slightly more efficient and performant.*/
	void doWorldRenderUpdate(float timeStep);

	void pauseWorld();
	void unPauseWorld();
	void onWindowResize(int width, int height);
	glm::vec2 getFrameSize();
	glm::vec2 getRenderFrameSize();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getCamPos();

	std::vector<struct PointLight*> getPointLights() { return pointLights; }
protected:
	aie::Input* input = nullptr;
	class GuiHud* guiHud = nullptr;
	class FlyCamera* cam = nullptr;
	bool hasDoneRenderUpdate = false;
	bool paused = false;
	float percentageToNextTick = 1.0F;

	/*Duration of time to pass between each fixed update*/
	float timeStep = 0.0333333F;
	bool initialized = false;
	std::vector<class TexturedBrush*> texturedBrushes;
	std::vector<class TexturedSurface*> texturedSurfaces;
	std::vector<class TexturedBrush*> rotatingTexturedBrushes;
	std::vector<struct PointLight*> pointLights;
	class TexturedModel* rabbetModel = nullptr;
	class TexturedModel* dragonModel = nullptr;
private:
	static Application3D* singletonInstance;
};