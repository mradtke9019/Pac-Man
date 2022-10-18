#include "Camera.h"

Camera::Camera()
	: WindowWidth(100), WindowHeight(600), FieldOfView(60.0f), CameraMinRange(0.1f), CameraMaxRange(100.f), pitch(0.0f), yaw(0.0f), roll(0.0f)
{

	pitch = 70.0f; 
	yaw = 0.0f;
	roll= 0.0f;

	CameraTranslateX = 0.0f;
	CameraTranslateY = 0.0f;
	CameraTranslateZ = 0.0f;

	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	Position = glm::vec3(0.0f, 3.0f, 3.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);
	Target = glm::vec3(0.0f, 0.0f, 0.0f) + cameraTranslation;
	
	// Default direction staring down z axis from positive to negative
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f,-1.0f);
	//cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cameraDirection.y = sin(glm::radians(pitch));
	//cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraDirection = glm::normalize(cameraDirection);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 Right = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 Up = glm::cross(cameraDirection, Right);



	View = glm::mat4(1.0f);

	View = glm::lookAt(Position, cameraDirection, Up);
};

glm::mat4 Camera::FixedViewCamera()
{

	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	Position = glm::vec3(0.0f, 3.0f, 3.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);
	Target = glm::vec3(0.0f, 0.0f, 0.0f) + cameraTranslation;

	// Default direction staring down z axis from positive to negative
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	//cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cameraDirection.y = sin(glm::radians(pitch));
	//cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraDirection = glm::normalize(cameraDirection);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 Right = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 Up = glm::cross(cameraDirection, Right);



	View = glm::mat4(1.0f);

	return glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), cameraDirection, Up);

}

glm::mat4 Camera::GetViewTransform()
{
	return View;
};