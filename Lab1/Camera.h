#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>


class Camera
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

	void CalculateDirection();

	void CalculateView();

	glm::mat4 GetViewTransform();

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