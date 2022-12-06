#pragma once

#include "ICamera.h"

class FreeCamera : public  ICamera
{
private:
	float yaw;
	float pitch;
	float roll;

	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;

	glm::vec3 Direction;

	glm::mat4 View;

public:
	GLfloat CameraTranslateX;
	GLfloat CameraTranslateY;
	GLfloat CameraTranslateZ;

	FreeCamera();
	FreeCamera(glm::vec3);

	glm::mat4 LookAt(glm::vec3, glm::vec3);

	void CalculateDirection();

	void CalculateView();

	glm::mat4* GetViewTransform();

	glm::vec3 GetPosition();

	void SetPosition(glm::vec3);

	glm::vec3 GetUp();

	void SetUp(glm::vec3);

	glm::vec3 GetTarget();

	void SetTarget(glm::vec3);

	void RotatePitch(float);

	void RotateYaw(float);

	void RotateRoll(float);
};