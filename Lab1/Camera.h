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
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Target;

	glm::mat4 View;
	glm::mat4 Projection;

	GLfloat CameraTranslateX;
	GLfloat CameraTranslateY;
	GLfloat CameraTranslateZ;

	int WindowWidth;
	int WindowHeight;
	float FieldOfView;

	float CameraMinRange;
	float CameraMaxRange;

	void CalculateViewTransform();
public:
	Camera();

	glm::mat4 GetViewTransform();

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3);

	glm::vec3 GetUp();
	void SetUp(glm::vec3);

	glm::vec3 GetTarget();
	void SetTarget(glm::vec3);
};

