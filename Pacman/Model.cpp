#include "Model.h"

Model::Model(std::string path, glm::vec3 Position, Shader* Shader)
	: ModelTransform(glm::mat4(1.0f))
{
	shader = Shader;
	ObjectColor = glm::vec3(1.0,0.0,0.0);
	ModelTransform = glm::translate(glm::mat4(1.0f), Position);
	this->Position = Position;
	LoadModel(path);
}

Model::Model(std::string path, glm::vec3 Position, Shader* Shader, glm::vec3 color)
	: ModelTransform(glm::mat4(1.0f))
{
	shader = Shader;
	ObjectColor = color;
	this->Position = Position;
	ModelTransform = glm::translate(glm::mat4(1.0f), Position);
	LoadModel(path);
}


void Model::Draw()
{
	shader->Use();
	shader->SetUniformMatrix4fv("model", &ModelTransform);
	shader->SetUniformVec3("ObjectColor", ObjectColor);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).Draw();
	}
}

void Model::Draw(Shader* shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).SetShader(shader);
		meshes.at(i).Draw();
	}
}

Shader* Model::GetShader()
{
	return shader;
}

void Model::SetShader(Shader* Shader)
{
	shader = Shader;
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i).SetShader(shader);
	}
}

glm::mat4 Model::GetModelTransform()
{
	return ModelTransform;
}

void Model::SetModelTransform(glm::mat4 model)
{
	ModelTransform = model;
}


void Model::SetColor(glm::vec3 color)
{
	this->ObjectColor = color;
}

glm::vec3 Model::GetColor()
{
	return this->ObjectColor;
}


glm::vec3 Model::GetPosition()
{
	return this->Position;
}