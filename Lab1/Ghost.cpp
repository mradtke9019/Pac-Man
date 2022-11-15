#pragma once

#include "Ghost.h"


Ghost::Ghost(glm::vec3 pos, Shader* shader)
	: modelPath("./Ghost.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), MoveSpeed(FastMoveSpeed()), mode(Attack)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}


void Ghost::Move(Player* player) 
{
	switch(mode)
	{
	case Attack:
		MoveTowardsPlayer(player);
		break;
	case Panic:
		MoveAwayFromPlayer(player);
		break;
	}
}

void Ghost::MoveTowardsPlayer(Player* player)
{
	glm::vec3 playerPosition = player->GetPosition();
	float currDistance = glm::distance(Position, playerPosition);
	float min = MAXINT32;
	Direction direction = Right;
	float d1 = glm::distance(glm::vec3(Position.x + MoveSpeed, Position.y, Position.z), playerPosition);
	float d2 = glm::distance(glm::vec3(Position.x - MoveSpeed, Position.y, Position.z), playerPosition);
	float d3 = glm::distance(glm::vec3(Position.x, Position.y, Position.z + MoveSpeed), playerPosition);
	float d4 = glm::distance(glm::vec3(Position.x, Position.y, Position.z - MoveSpeed), playerPosition);
	float d0 = glm::distance(glm::vec3(Position.x, Position.y, Position.z), playerPosition);

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
	if (d0 < min){
		min = d0;
		direction = None;
	}

	switch (direction)
	{
	case Up:
		MoveUp();
		break;
	case Down:
		MoveDown();
		break;
	case Left:
		MoveLeft();
		break;
	case Right:
		MoveRight();
		break;
	case None:
		break;
	}
}

void Ghost::MoveAwayFromPlayer(Player* player)
{
	glm::vec3 playerPosition = player->GetPosition();
	float currDistance = glm::distance(Position, playerPosition);
	float max = MININT32;
	Direction direction = Right;
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

	switch (direction)
	{
	case Up:
		MoveUp();
		break;
	case Down:
		MoveDown();
		break;
	case Left:
		MoveLeft();
		break;
	case Right:
		MoveRight();
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

void Ghost::MoveUp()
{
	Position.z-= MoveSpeed;
}
void Ghost::MoveDown()
{
	Position.z+= MoveSpeed;
}
void Ghost::MoveLeft()
{
	Position.x-= MoveSpeed;
}
void Ghost::MoveRight()
{
	Position.x+= MoveSpeed;
}