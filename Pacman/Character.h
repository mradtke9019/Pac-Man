#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

class Character
{
private:
	Model* model;
	glm::vec3 Position;
	std::string modelPath;
	float rotateX;
	float rotateY;
	float rotateZ;



public:

	virtual glm::mat4 GetModelTransform()
	{
		return glm::mat4(1.0f) *
			glm::translate(glm::mat4(1.0f), Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotateY), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	virtual void Draw()
	{
		model->SetModelTransform(GetModelTransform());
		model->Draw();
	}

	virtual Model* GetModel()
	{
		return model;
	}

	virtual void MoveUp(float x)
	{
		rotateY = 90.f;
		rotateZ = 270.0f;
		Position.z--;
	}
	virtual void MoveDown(float x)
	{
		rotateY = 90.f;
		rotateZ = 90.0f;
		Position.z++;
	}
	virtual void MoveLeft(float x)
	{
		rotateY = 90.f;
		rotateZ = 180.0f;
		Position.x--;
	}
	virtual void MoveRight(float x)
	{
		rotateY = -90.f;
		rotateZ = 0.0f;
		Position.x++;
	}
};