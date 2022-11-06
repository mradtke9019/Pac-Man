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

// Camera interface where all cameras must return a view transform
class ICamera
{
private:

public:

	virtual glm::mat4 GetViewTransform() = 0;
	virtual glm::vec3 GetPosition() = 0;

	virtual void TranslateX(float) = 0;
	virtual void TranslateY(float) = 0;
	virtual void TranslateZ(float) = 0;

	virtual void RotateYaw(float) = 0;

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
};