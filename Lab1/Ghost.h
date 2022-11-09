#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.h"
#include "Model.h"

class Ghost
{
private:
	Model* model;
	glm::vec3 Position;
	std::string modelPath;
	float yaw;
	float pitch;
	float roll;

	glm::mat4 GetModelTransform()
	{
		//glm::rotate(glm::mat4(1.0f), RotateZ, glm::vec3(0.0f, 0.0f, 1.0f))*
		//	glm::rotate(glm::mat4(1.0f), RotateY, glm::vec3(0.0f, 1.0f, 0.0f))*
		//	glm::rotate(glm::mat4(1.0f), RotateX, glm::vec3(1.0f, 0.0f, 0.0f));
		return glm::mat4(1.0f) *
			glm::translate(glm::mat4(1.0f), Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}

public:
	Ghost(glm::vec3 pos, Shader* shader);
	Model* GetModel();
	void MoveTowardsPlayer(Player* player, float x);
	void MoveUp(float x);
	void MoveDown(float x);
	void MoveLeft(float x);
	void MoveRight(float x);
	void Draw();
};