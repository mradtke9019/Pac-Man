#include "Camera.h"

Camera::Camera()
	: pitch(0.0f), yaw(0.0f), roll(0.0f)
{

	pitch = 0.0f; 
	yaw = -90.0f;
	roll= 0.0f;

	CameraTranslateX = 0.0f;
	CameraTranslateY = 0.0f;
	CameraTranslateZ = 0.0f;

	Position = glm::vec3(0.0f, 3.0f, 3.0f);
	CalculateView();
	//glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	//Position = glm::vec3(0.0f, 3.0f, 3.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);
	//Target = glm::vec3(0.0f, 0.0f, 0.0f) + cameraTranslation;
	//
	//// Default direction staring down z axis from positive to negative
	//Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	//Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//Direction.y = sin(glm::radians(pitch));
	//Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//Direction = glm::normalize(Direction);

	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	//glm::vec3 Right = glm::normalize(glm::cross(up, Direction));
	//Up = glm::cross(Direction, Right);

	//View = glm::lookAt(Position, Direction, Up);
};

void Camera::CalculateDirection()
{
	Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction.y = sin(glm::radians(pitch));
	Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction = glm::normalize(Direction);
}

void Camera::CalculateView()
{
	CalculateDirection();
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::normalize(glm::cross(up, Direction));
	Up = glm::cross(Direction, Right);

	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	Position = glm::vec3(0.0f, 3.0f, 6.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);

	View = glm::lookAt(Position, Position + Direction, Up);
}

glm::mat4 Camera::FixedViewCamera()
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	return glm::lookAt(cameraPos, cameraTarget, cameraUp);
}

glm::mat4 Camera::GetViewTransform()
{
	CalculateView();
	return View;
};

void Camera::RotatePitch(float theta)
{
	pitch += theta;
	CalculateView();
}

void Camera::RotateYaw(float theta)
{
	yaw += theta;
	CalculateView();
}
void Camera::RotateRoll(float theta) 
{

}