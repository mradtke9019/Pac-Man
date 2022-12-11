#include "Mesh.h"
// Inspiration for Mesh Class and cincepts here
// https://learnopengl.com/Model-Loading/Mesh


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	unsigned int vPosition = shader->GetAttribLocation("vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	unsigned int vNormal = shader->GetAttribLocation("vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	unsigned int vTexture = shader->GetAttribLocation("vTexture");
	glEnableVertexAttribArray(vTexture);
	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Shader* shader)
{
	this->shader = shader;

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}



void Mesh::SetShader(Shader* shader)
{
	this->shader = shader;
}

Shader* Mesh::GetShader()
{
	return shader;
}

void Mesh::Draw()
{
	shader->Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}