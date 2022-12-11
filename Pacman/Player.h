#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Direction.h"
#include "Arena.h"

class Player
{
private:
	Model* model;
	glm::vec3 Position;
	std::string modelPath;
	float yaw;
	float pitch;
	float roll;
	float movespeed;
	float deltaTime;
	Direction direction;

	glm::mat4 GetModelTransform()
	{
		return glm::mat4(1.0f) * 
			glm::translate(glm::mat4(1.0f), Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(1.0f,0.0f,0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
	}

public: 
	Player(glm::vec3 pos, Shader* shader);
	Model* GetModel();
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 Position);
	void SetDeltaTime(float dT);
	void SetDirection(Direction direction);
	void Move(Arena* arena);
	void MoveUp(Arena* arena);
	void MoveDown(Arena* arena);
	void MoveLeft(Arena* arena);
	void MoveRight(Arena* arena);
	void Draw();
};