#include "FlyCamera.h"
#include "Input.h"
#include "MouseHelper.h"
FlyCamera::~FlyCamera()
{
}

void FlyCamera::onWorldFixedUpdate(float timeStep)
{
	Camera::onWorldFixedUpdate(timeStep);
	position += frontVec * moveVec.z * moveSpeed * timeStep;
	position += rightVec * moveVec.x * moveSpeed * timeStep;
	position.y += moveVec.y * moveSpeed * timeStep;
	moveVec = { 0,0,0 };
}

void FlyCamera::onUpdate(float ptnt, float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	//get input and update pitch,yaw and movevec here
	if (moveVec.x == 0.0F) moveVec.x = (int)(input->isKeyDown(aie::INPUT_KEY_D)) - (int)(input->isKeyDown(aie::INPUT_KEY_A));
	if (moveVec.y == 0.0F) moveVec.z = (int)(input->isKeyDown(aie::INPUT_KEY_W)) - (int)(input->isKeyDown(aie::INPUT_KEY_S));
	if (moveVec.z == 0.0F) moveVec.y = (int)(input->isKeyDown(aie::INPUT_KEY_SPACE)) - (int)(input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL));
	if (moveVec != glm::vec3(0,0,0)) glm::normalize(moveVec);

	pitch += MouseHelper::get()->getYDelta() * mouseSensitivity;
	yaw += MouseHelper::get()->getXDelta() * mouseSensitivity;

	if (pitch >= 90.0F)
	{
		pitch = 89.9999F;
	}
	else if (pitch <= -90.0F)
	{
		pitch = -89.9999F;
	}

	Camera::onUpdate(ptnt, deltaTime);
}
