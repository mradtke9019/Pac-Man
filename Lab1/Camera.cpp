#include "Camera.h"

Camera::Camera()
	: WindowWidth(100), WindowHeight(600), FieldOfView(60.0f), CameraMinRange(0.1f), CameraMaxRange(100.f)
{
	WindowWidth = 800;
	WindowHeight = 600;
	FieldOfView = 60.0f;

	CameraMinRange = 0.1f;
	CameraMaxRange = 100.0f;

	CameraTranslateX = 0.0f;
	CameraTranslateX = 0.0f;
	CameraTranslateX = 0.0f;

	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	Position = glm::vec3(0.0f, 3.0f, 3.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);
	Target = glm::vec3(0.0f, 0.0f, 0.0f) + cameraTranslation;

	glm::vec3 cameraDirection = glm::normalize(Position - Target);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 Right = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 Up = glm::cross(cameraDirection, Right);



	View = glm::mat4(1.0f);
	Projection = glm::mat4(1.0f);


	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	View = glm::lookAt(Position, Target, Up);
	Projection = glm::perspective(glm::radians(FieldOfView), (float)WindowWidth / (float)WindowHeight, CameraMinRange, CameraMaxRange);
};

glm::mat4 Camera::GetViewTransform()
{
	return View;
};