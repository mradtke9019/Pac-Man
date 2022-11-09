#pragma once

#include "Ghost.h"

enum Direction { Up, Down, Left, Right };

Ghost::Ghost(glm::vec3 pos, Shader* shader)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}

void Ghost::MoveTowardsPlayer(Player* player, float x)
{
	glm::vec3 playerPosition = player->GetPosition();
	float currDistance = glm::distance(Position, playerPosition);
	float min = MAXINT32;
	Direction direction = Right;
	float d1 = glm::distance(glm::vec3(Position.x + x, Position.y, Position.z), playerPosition);
	float d2 = glm::distance(glm::vec3(Position.x - x, Position.y, Position.z), playerPosition);
	float d3 = glm::distance(glm::vec3(Position.x, Position.y, Position.z + x), playerPosition);
	float d4 = glm::distance(glm::vec3(Position.x, Position.y, Position.z - x), playerPosition);

	if (d1 < min) 
	{
		min = d1;
		direction = Right;
	}
	if (d2 < min) 
	{
		min = d2;
		direction = Left;
	}
	if (d3 < min) 
	{
		min = d3;
		direction = Down;
	}
	if (d4 < min) {
		min = d4;
		direction = Up;
	}

	switch (direction)
	{
	case Up:
		MoveUp(x);
		break;
	case Down:
		MoveDown(x);
		break;
	case Left:
		MoveLeft(x);
		break;
	case Right:
		MoveRight(x);
		break;
	}
}


void Ghost::Draw()
{
	model->SetModelTransform(GetModelTransform());
	model->Draw();
}

Model* Ghost::GetModel()
{
	return model;
}

void Ghost::MoveUp(float x)
{
	Position.z-= x;
}
void Ghost::MoveDown(float x)
{
	Position.z+=x;
}
void Ghost::MoveLeft(float x)
{
	Position.x-=x;
}
void Ghost::MoveRight(float x)
{
	Position.x+=x;
}