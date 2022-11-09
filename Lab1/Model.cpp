#include "Model.h"

Model::Model(std::string path, glm::vec3 Position, Shader* Shader)
	: ModelTransform(glm::mat4(1.0f))
{
	shader = Shader;
	LoadModel(path);
}

void Model::Draw()
{
	shader->Use();
	shader->SetUniformMatrix4fv("model", &ModelTransform);
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