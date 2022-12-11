#include "FreeCamera.h"

FreeCamera::FreeCamera(glm::vec3 position)
	: pitch(0.0f), yaw(-90.0f), roll(0.0f)
{

	//pitch = 0.0f;
	//yaw = -90.0f;
	//roll = 0.0f;

	CameraTranslateX = 0.0f;
	CameraTranslateY = 0.0f;
	CameraTranslateZ = 0.0f;

	Position = position;// glm::vec3(0.0f, 3.0f, 3.0f);
	CalculateView();
};

void FreeCamera::CalculateDirection()
{
	Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction.y = sin(glm::radians(pitch));
	Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Direction = glm::normalize(Direction);
}

glm::mat4 FreeCamera::LookAt(glm::vec3 Target, glm::vec3 up)
{
	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);

	return glm::lookAt(Position + cameraTranslation, Target, up);
}

void FreeCamera::CalculateView()
{
	CalculateDirection();
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::normalize(glm::cross(up, Direction));
	Up = glm::cross(Direction, Right);

	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	Position = glm::vec3(0.0f, 3.0f, 6.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);

	View = glm::lookAt(Position, Position + Direction, Up);
}

glm::vec3 FreeCamera::GetPosition()
{
	return Position;
}

glm::mat4* FreeCamera::GetViewTransform()
{
	CalculateView();
	return &View;
};

void FreeCamera::RotatePitch(float theta)
{
	pitch += theta;
	CalculateView();
}

void FreeCamera::RotateYaw(float theta)
{
	yaw += theta;
	CalculateView();
}
void FreeCamera::RotateRoll(float theta)
{

}