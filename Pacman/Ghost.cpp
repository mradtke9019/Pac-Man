#pragma once

#include "Ghost.h"


Ghost::Ghost(glm::vec3 pos, Shader* shader)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack), Momentum(50), MomentumCount(0), deltaTime(0.0f), panicTime(0.0f)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}

Ghost::Ghost(glm::vec3 pos, Shader* shader, int Momentum)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack), MomentumCount(0), deltaTime(0.0f), panicTime(0.0f)
{
	Position = pos;
	this->Momentum = Momentum;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetColor(glm::vec3(1.0, 1.0, 1.0));
	model->SetModelTransform(GetModelTransform());
}


Ghost::Ghost(glm::vec3 pos, Shader* shader, int Momentum, glm::vec3 color)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack), MomentumCount(0), deltaTime(0.0f), panicTime(0.0f)
{
	Position = pos;
	this->Momentum = Momentum;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetColor(color);
	model->SetModelTransform(GetModelTransform());
}


void Ghost::SetDeltaTime(float dT)
{
	this->deltaTime = dT;
}

void Ghost::SetPosition(glm::vec3 Position)
{
	this->Position = Position;
}

void Ghost::Move(Player* player, Arena* arena, Shader* shader)
{
	model->SetShader(shader);
	switch(mode)
	{
	case Attack:
		MoveTowardsPlayer(player, arena);
		break;
	case Panic:
		panicTime += deltaTime;
		if (panicTime > panicThreshold())
		{
			panicTime = 0.0f;
			mode = Attack;
		}
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
		MomentumCount = 0;

		glm::vec2 playerCoordinate = arena->WorldToGrid(player->GetPosition());
		glm::vec2 ghostCoordinate = arena->WorldToGrid(Position);

		direction = SearchAgent::GetDirectionOfShortestPath(
			std::pair<int, int>(ghostCoordinate.x, ghostCoordinate.y),
			std::pair<int, int>(playerCoordinate.x, playerCoordinate.y),
			(arena->GetValidPathing()));
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
		MomentumCount = 0;

		glm::vec2 playerCoordinate = arena->WorldToGrid(player->GetPosition());
		glm::vec2 ghostCoordinate = arena->WorldToGrid(Position);

		direction = SearchAgent::GetDirectionOfShortestPath(
			std::pair<int, int>(ghostCoordinate.x, ghostCoordinate.y),
			std::pair<int, int>(playerCoordinate.x, playerCoordinate.y),
			(arena->GetValidPathing()));
		direction = OppositeDirection(direction);
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
	model->GetShader()->Use();
	model->SetModelTransform(GetModelTransform());
	model->Draw();
}

Model* Ghost::GetModel()
{
	return model;
}

void Ghost::MoveUp(Arena* arena)
{
	glm::vec3 destination = Position - glm::vec3(0, 0, MoveSpeed * deltaTime);


	bool valid = arena->IsNavigatable(destination);
	if (valid) {
		// If i am moving vertically, I want to clamp my position to the midpoint horizontally
		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z -= MoveSpeed * deltaTime;
	}

}
void Ghost::MoveDown(Arena* arena)
{
	glm::vec3 destination = Position + glm::vec3(0, 0, MoveSpeed * deltaTime);
	bool valid = arena->IsNavigatable(destination);

	if (valid) {

		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z += MoveSpeed * deltaTime;
	}
}
void Ghost::MoveLeft(Arena* arena)
{
	glm::vec3 destination = Position - glm::vec3(MoveSpeed * deltaTime, 0, 0);
	bool valid = arena->IsNavigatable(destination);
	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x -= MoveSpeed * deltaTime;
	}
}
void Ghost::MoveRight(Arena* arena)
{
	glm::vec3 destination = Position + glm::vec3(MoveSpeed * deltaTime, 0, 0);
	bool valid = arena->IsNavigatable(destination);

	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x += MoveSpeed * deltaTime;
	}
}


glm::vec3 Ghost::GetPosition()
{
	return Position;
}