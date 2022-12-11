#pragma once
#include "ICamera.h"

class FixedCamera : public ICamera
{
private:
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;

	glm::mat4 View;

public:

	FixedCamera(){};
	FixedCamera(glm::vec3, glm::vec3, glm::vec3);

	glm::mat4* GetViewTransform();

	glm::vec3 GetPosition();

	void SetPosition(glm::vec3);

	glm::vec3 GetTarget();

	void SetTarget(glm::vec3);

	// Dont implement these for a fixed camera
	void TranslateX(float x) {};
	void TranslateY(float x) {};
	void TranslateZ(float x) {};
	void RotateYaw(float x) {};
};