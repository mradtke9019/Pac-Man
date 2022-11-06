#include "Object.h"

Object::Object(GLfloat vertices[], GLfloat colors[], int numVertices, Shader* Shader, glm::vec3 Position)
	: VBO(0), VAO(0), NumVertices(0), NumTriangles(0), shader(0)
{
	shader = Shader;
	NumVertices = numVertices;
	position = Position;
	generateObjectBuffer(vertices, colors);
	modelTransform = glm::translate(glm::mat4(1.0f), position);
}

Object::Object(glm::vec3 vertices[], glm::vec4 colors[], int numVertices, Shader* Shader, glm::vec3 Position)
	: VBO(0), VAO(0), NumVertices(0), NumTriangles(0), shader(0)
{
	shader = Shader;
	position = Position;
	NumVertices = numVertices;
	generateObjectBuffer(vertices, colors);
	modelTransform = glm::translate(glm::mat4(1.0f), position);
}

Object::Object(std::string objectPath, Shader* Shader)
	: VBO(0), VAO(0), NumVertices(0), NumTriangles(0), shader(0)
{
	shader = Shader;
	generateObjectBufferMesh(objectPath);
}

Shader* Object::GetShader()
{
	return shader;
}

void Object::Object::AssignShader(Shader* Shader)
{
	this->shader = Shader;
}

glm::mat4 Object::GetModelTransform()
{
	return modelTransform;
}

void Object::SetModelTransform(glm::mat4 Model)
{
	modelTransform = Model;
}

void Object::Draw()
{
	shader->Use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	shader->SetUniformMatrix4fv("model", &modelTransform);
	linkCurrentBuffertoShader();
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}