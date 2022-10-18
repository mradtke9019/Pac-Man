#include "Model.h"

Model::Model(std::string path, Shader* Shader)
{
	shader = Shader;
	LoadModel(path);
}

void Model::Draw()
{
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
}

glm::mat4 Model::GetModelTransform()
{
	return ModelTransform;
}

void Model::SetModelTransform(glm::mat4 model)
{
	ModelTransform = model;
}