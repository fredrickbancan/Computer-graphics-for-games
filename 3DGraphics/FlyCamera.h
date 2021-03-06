#pragma once
#include "Camera.h"

/*Class for movable 3d Camera*/
class FlyCamera : public Camera
{
public:
	FlyCamera(glm::vec3 pos) : Camera(pos) {};
	~FlyCamera();
	virtual void onWorldFixedUpdate(float timeStep) override;
	virtual void onUpdate(float ptnt, float deltaTime) override; 
	float getMoveSpeed() { return moveSpeed; }
	void setMoveSpeed(float ms) { moveSpeed  = ms; }
	float setMouseSense(float ms) { mouseSensitivity = ms; }
	glm::vec3 getMoveVec() { return moveVec; }
protected:
	float moveSpeed = 3.0F;
	float mouseSensitivity = 0.075F;
	glm::vec3 moveVec = {0,0,0}; //vector representing user input
};