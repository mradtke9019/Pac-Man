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
};