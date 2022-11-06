#pragma once

#include "Player.h"


Player::Player(glm::vec3 pos, Shader* shader)
	: modelPath("./Pacman.obj"), model(nullptr), yaw(-90.0f), pitch(0), roll(90.f)
{
	Position = pos;
	model = new Model(modelPath, Position, shader);
	// want to flip the pacman model on its side
	model->SetModelTransform(GetModelTransform());
}


void Player::Draw()
{
	model->SetModelTransform(GetModelTransform());
	model->Draw();
}

Model* Player::GetModel()
{
	return model;
}

void Player::MoveUp(float x)
{
	yaw = 90.f;
	pitch = 270.0f;
	Position.z--;
}
void Player::MoveDown(float x)
{
	yaw = 90.f;
	pitch = 90.0f;
	Position.z++;
}
void Player::MoveLeft(float x)
{
	yaw = 90.f;
	pitch = 180.0f;
	Position.x--;
}
void Player::MoveRight(float x)
{
	yaw = -90.f;
	pitch = 0.0f;
	Position.x++;
}