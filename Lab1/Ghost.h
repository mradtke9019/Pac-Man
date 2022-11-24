#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.h"
#include "Model.h"
#include "Direction.h"
#include "SearchAgent.h"

enum Mode {Panic, Attack};

class Ghost
{
private:
	Mode mode;
	Model* model;
	glm::vec3 Position;
	std::string modelPath;
	float yaw;
	float pitch;
	float roll;
	float MoveSpeed;
	int Momentum;
	int MomentumCount;
	Direction direction;

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
	Ghost(glm::vec3 pos, Shader* shader, int Momentum);
	static float FastMoveSpeed()
	{
		return 0.5f;
	}
	static float SlowMoveSpeed()
	{
		return 1.05f;
	}
	Model* GetModel();
	void Move(Player* player, Arena* arena);
	void MoveTowardsPlayer(Player* player, Arena* arena);
	void MoveAwayFromPlayer(Player* player, Arena* arena);
	void SetMovespeed(float x);
	float GetMovespeed();
	void SetMode(Mode Mode);
	Mode GetMode();
	void MoveUp(Arena* arena);
	void MoveDown(Arena* arena);
	void MoveLeft(Arena* arena);
	void MoveRight(Arena* arena);
	void Draw();
};