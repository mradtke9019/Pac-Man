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
	float panicTime;
	Model* model;
	glm::vec3 Position;
	std::string modelPath;
	float yaw;
	float pitch;
	float roll;
	float MoveSpeed;
	float deltaTime;
	int Momentum;
	int MomentumCount;
	Direction direction;
	glm::mat4 GetModelTransform()
	{
		return glm::mat4(1.0f) *
			glm::translate(glm::mat4(1.0f), Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}

public:
	Ghost(glm::vec3 pos, Shader* shader);
	Ghost(glm::vec3 pos, Shader* shader, int Momentum);
	Ghost(glm::vec3 pos, Shader* shader, int Momentum, glm::vec3 color);

	static float panicThreshold() 
	{
		return 5000.0f;
	}
	static float FastMoveSpeed()
	{
		return 0.03f;
	}
	static float SlowMoveSpeed()
	{
		return 0.03f;
	}
	Model* GetModel();
	void SetPosition(glm::vec3 Position);
	void SetDeltaTime(float dT);
	void Move(Player* player, Arena* arena, Shader* shader);
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
	glm::vec3 GetPosition();
};