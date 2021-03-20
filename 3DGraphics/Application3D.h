#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Input.h"
#include <vector>
class Application3D : public aie::Application 
{
public:
	static void start() { deleteInstance(); singletonInstance = new Application3D(); singletonInstance->run("Bootstrap 3D Graphics", 640, 480, false); }
	static Application3D* getInstance() { return singletonInstance; }
	static void deleteInstance() { if (singletonInstance) { delete singletonInstance; } }

	virtual bool startup();
	virtual void shutdown();
	virtual void update(float deltaTime);
	virtual void draw();

	void doWorldFixedUpdateLoop(float deltaTime);
	void doWorldFixedUpdate(float timeStep);
	void doWorldRenderUpdate(float timeStep);
	void pauseWorld();
	void unPauseWorld();
	void onWindowResize(int width, int height);
	glm::vec2 getFrameSize();
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
	float timeStep = 0.0333333F;
	bool initialized = false;
	std::vector<class TexturedBrush*> texturedBrushes;
	std::vector<struct PointLight*> pointLights;
private:
	static Application3D* singletonInstance;
};