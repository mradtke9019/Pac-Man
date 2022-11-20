#pragma once

#include "Player.h"


Player::Player(glm::vec3 pos, Shader* shader)
	: modelPath("./Pacman.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f), movespeed(1.0f), direction(None)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}


void Player::Draw()
{
	model->GetShader()->Use();
	model->SetModelTransform(GetModelTransform());
	model->Draw();
}

glm::vec3 Player::GetPosition()
{
	return Position;
}

void Player::SetDirection(Direction direction)
{
	this->direction = direction;
}

Model* Player::GetModel()
{
	return model;
}

void Player::Move(Arena* arena)
{
	switch (this->direction)
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
// Function that takes a pointer
// to a function
int invoke(int x, int y, int (*func)(int, int))
{
	return func(x, y);
}
void Player::MoveUp(Arena* arena)
{
	yaw = 90.f;
	pitch = 270.0f;
	glm::vec3 destination = Position - glm::vec3(0, 0, movespeed);


	bool valid = arena->IsNavigatable(destination);
	if (valid) {
		// If i am moving vertically, I want to clamp my position to the midpoint horizontally
		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z -= movespeed;
	}
}
void Player::MoveDown(Arena* arena)
{
	yaw = 90.f;
	pitch = 90.0f;

	glm::vec3 destination = Position + glm::vec3(0, 0, movespeed);
	bool valid = arena->IsNavigatable(destination);

	if (valid) {

		Position.x = arena->GetNearestValidPosition(destination).x;
		Position.z += movespeed;
	}
}
void Player::MoveLeft(Arena* arena)
{
	yaw = 90.f;
	pitch = 180.0f;

	glm::vec3 destination = Position - glm::vec3(movespeed, 0, 0);
	bool valid = arena->IsNavigatable(destination);
	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x -= movespeed;
	}
}
void Player::MoveRight(Arena* arena)
{
	yaw = -90.f;
	pitch = 0.0f;	

	glm::vec3 destination = Position + glm::vec3(movespeed, 0, 0);
	bool valid = arena->IsNavigatable(destination);

	if (valid)
	{
		Position.z = arena->GetNearestValidPosition(destination).z;
		Position.x += movespeed;
	}
}