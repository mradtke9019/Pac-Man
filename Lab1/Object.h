#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h> // scene importer


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


class Object
{
private:
	Shader* shader;
	GLuint VBO;
	int NumVertices;
	int NumTriangles;
	glm::mat4 modelTransform;
	glm::vec3 position;

	GLuint generateObjectBuffer(GLfloat vertices[], GLfloat colors[]) {
		// Genderate 1 generic buffer object, called VBO
		GLuint VBO;
		glGenBuffers(1, &VBO);
		// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
		// Buffer will contain an array of vertices 
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, NumVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, NumVertices * 3 * sizeof(GLfloat), vertices);
		glBufferSubData(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat), NumVertices * 4 * sizeof(GLfloat), colors);
		return VBO;
	}

	GLuint generateObjectBuffer(glm::vec3 vertices[], glm::vec4 colors[])
	{
		GLuint VBO;
		glGenBuffers(1, &VBO);
		// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
		// Buffer will contain an array of vertices
		size_t vertexMemory = NumVertices * sizeof(glm::vec3);
		size_t colorMemory = NumVertices * sizeof(glm::vec4);
		size_t memoryTotal = vertexMemory + colorMemory;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, memoryTotal, NULL, GL_STATIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexMemory, vertices);
		glBufferSubData(GL_ARRAY_BUFFER, vertexMemory, colorMemory, colors);
		return VBO;
	}

	void linkCurrentBuffertoShader() {
		// find the location of the variables that we will be using in the shader program
		GLuint positionID = shader->GetAttribLocation("vPosition");
		GLuint colorID = shader->GetAttribLocation("vColor");
		// Have to enable this
		glEnableVertexAttribArray(positionID);
		// Tell it where to find the position data in the currently active buffer (at index positionID)
		glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// Similarly, for the color data.
		glEnableVertexAttribArray(colorID);
		glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NumVertices * 3 * sizeof(GLfloat)));
	}

public:

	Object(GLfloat vertices[], GLfloat colors[], int numVertices, Shader* Shader, glm::vec3 Position);

	Object(glm::vec3 vertices[], glm::vec4 colors[], int numVertices, Shader* Shader, glm::vec3 Position);

	Shader* GetShader();

	void AssignShader(Shader* Shader);

	glm::mat4 GetModelTransform();

	void SetModelTransform(glm::mat4 Model);

	void Draw();
};