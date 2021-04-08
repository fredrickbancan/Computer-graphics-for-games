#pragma once
#include "glm/vec3.hpp"
#include "glm/ext.hpp"

/*Class for a 3D camera with projection and view matrices*/
class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 lookAt);
	Camera(glm::vec3 pos, float pitch, float yaw);
	Camera(glm::vec3 pos);
	~Camera();
	virtual void onWorldFixedUpdate(float timeStep);
	virtual void onUpdate(float ptnt, float deltaTime);
	glm::vec3 getPosition() { return position; }
	glm::vec3 getLerpPosition(float ptnt) { return prevTickPosition + (position - prevTickPosition) * ptnt; }
	float getYaw() { return yaw; }
	float getPitch() { return pitch; }
	void lookAt(glm::vec3 pos, float ptnt = 1.0F);

	/*Update the directional vectors of the camera based on position rotations. ptnt = percentage to next tick, for interpolating*/
	void updateVectors(float ptnt = 1.0F);

	glm::mat4 getViewMatrix() { return viewMatrix; }
	glm::mat4 getProjectionMatrix() { return projectionMatrix; }
	void setFov(float fov);
	void setNearPlane(float np);
	void setFarPlane(float fp);

	/*Calcultes projection matrix. Should be called each time the aspect ratio, fov or draw distance changes.*/
	void makeProjectionMatrix();

	void setAspectRatio(float ar) { aspectRatio = ar; makeProjectionMatrix(); }
protected:
	float farPlane = 1000.0F;
	float nearPlane = 0.1F;
	float fov = 75.0F;
	float yaw;
	float pitch;
	glm::vec3 prevTickPosition;
	glm::vec3 position;
	glm::vec3 frontVec;
	glm::vec3 rightVec;
	const glm::vec3 upVec = {0,1,0};
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	float aspectRatio = 1920 / 1080;
};