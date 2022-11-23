#pragma once

#include "Ghost.h"


Ghost::Ghost(glm::vec3 pos, Shader* shader)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack), Momentum(50), MomentumCount(0)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}

Ghost::Ghost(glm::vec3 pos, Shader* shader, int Momentum)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack), MomentumCount(0)
{
	Position = pos;
	this->Momentum = Momentum;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}

void Ghost::Move(Player* player, Arena* arena)
{
	switch(mode)
	{
	case Attack:
		MoveTowardsPlayer(player, arena);
		break;
	case Panic:
		MoveAwayFromPlayer(player, arena);
		break;
	}
}

void Ghost::MoveTowardsPlayer(Player* player, Arena* arena)
{

	if (MomentumCount < Momentum)
	{
		MomentumCount++;
	}
	else
	{
		glm::vec3 playerPosition = player->GetPosition();
		float currDistance = glm::distance(Position, playerPosition);
		float min = MAXINT32;
		MomentumCount = 0;
		glm::vec3 right = glm::vec3(Position.x + MoveSpeed, Position.y, Position.z);
		glm::vec3 left = glm::vec3(Position.x - MoveSpeed, Position.y, Position.z);
		glm::vec3 down = glm::vec3(Position.x, Position.y, Position.z + MoveSpeed);
		glm::vec3 up = glm::vec3(Position.x, Position.y, Position.z - MoveSpeed);
		float d1 = glm::distance(right, playerPosition);
		float d2 = glm::distance(left, playerPosition);
		float d3 = glm::distance(down, playerPosition);
		float d4 = glm::distance(up, playerPosition);
		float d0 = glm::distance(glm::vec3(Position.x, Position.y, Position.z), playerPosition);

 		if (d1 < min && arena->IsNavigatable(right))
		{
			min = d1;
			direction = Right;
		}
		if (d2 < min && arena->IsNavigatable(left))
		{
			min = d2;
			direction = Left;
		}
		if (d3 < min && arena->IsNavigatable(down))
		{
			min = d3;
			direction = Down;
		}
		if (d4 < min && arena->IsNavigatable(up)) {
			min = d4;
			direction = Up;
		}
		if (d0 < min) {
			min = d0;
			direction = None;
		}

	}
	switch (direction)
	{
	case Up:
		MoveUp(arena);
		break;
	case Down:
		MoveDown(arena);
		break;
	case Left:
		MoveLeft(arena);
		break;
	case Right:
		MoveRight(arena);
		break;
	case None:
		break;
	}
}

void Ghost::MoveAwayFromPlayer(Player* player, Arena* arena)
{
	if (MomentumCount < Momentum)
	{
		MomentumCount++;
	}
	else 
	{
		glm::vec3 playerPosition = player->GetPosition();
		float currDistance = glm::distance(Position, playerPosition);
		float max = MININT32;

		MomentumCount = 0;
		float d1 = glm::distance(glm::vec3(Position.x + MoveSpeed, Position.y, Position.z), playerPosition);
		float d2 = glm::distance(glm::vec3(Position.x - MoveSpeed, Position.y, Position.z), playerPosition);
		float d3 = glm::distance(glm::vec3(Position.x, Position.y, Position.z + MoveSpeed), playerPosition);
		float d4 = glm::distance(glm::vec3(Position.x, Position.y, Position.z - MoveSpeed), playerPosition);

		if (d1 > max)
		{
			max = d1;
			direction = Right;
		}
		if (d2 > max)
		{
			max = d2;
			direction = Left;
		}
		if (d3 > max)
		{
			max = d3;
			direction = Down;
		}
		if (d4 > max) {
			max = d4;
			direction = Up;
		}
	}

	switch (direction)
	{
	case Up:
		MoveUp(arena);
		break;
	case Down:
		MoveDown(arena);
		break;
	case Left:
		MoveLeft(arena);
		break;
	case Right:
		MoveRight(arena);
		break;
	case None:
		break;
	}
}


void Ghost::SetMovespeed(float x)
{
	MoveSpeed = x;
}
float Ghost::GetMovespeed()
{
	return MoveSpeed;
}

void Ghost::SetMode(Mode Mode)
{
	mode = Mode;
}

Mode Ghost::GetMode()
{
	return mode;
}

void Ghost::Draw()
{
	model->SetModelTransform(GetModelTransform());
	model->GetShader()->Use();
	model->Draw();
}

Model* Ghost::GetModel()
{
	return model;
}

void Ghost::MoveUp(Arena* arena)
{
	glm::vec3 destination = Position - glm::vec3(0, 0, MoveSpeed);


	bool valid = arena->IsNavigatable(destination);
	if (valid) {
		// If i am moving vertically, I want to clamp my position to the midpoint horizontally
		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z -= MoveSpeed;
	}

}
void Ghost::MoveDown(Arena* arena)
{
	glm::vec3 destination = Position + glm::vec3(0, 0, MoveSpeed);
	bool valid = arena->IsNavigatable(destination);

	if (valid) {

		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z += MoveSpeed;
	}
}
void Ghost::MoveLeft(Arena* arena)
{
	glm::vec3 destination = Position - glm::vec3(MoveSpeed, 0, 0);
	bool valid = arena->IsNavigatable(destination);
	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x -= MoveSpeed;
	}
}
void Ghost::MoveRight(Arena* arena)
{
	glm::vec3 destination = Position + glm::vec3(MoveSpeed, 0, 0);
	bool valid = arena->IsNavigatable(destination);

	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x += MoveSpeed;
	}
}