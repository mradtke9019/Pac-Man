#pragma once
#include "ICamera.h"

// Ideas and inspiration from https://learnopengl.com/Getting-started/Camera
class Camera : public ICamera
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

	Camera();
	Camera(glm::vec3);

	glm::mat4 LookAt(glm::vec3, glm::vec3);

	static glm::mat4 FixedViewCamera()
	{
		glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 5.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		return glm::lookAt(cameraPos, cameraTarget, cameraUp);
	}

	void TranslateX(float);
	void TranslateY(float);
	void TranslateZ(float);

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