#include "Shader.cpp"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#pragma once
class Object
{
private:
	Shader* shader;
	int VBO;
	int NumVertices;
	int NumTriangles;

	GLuint generateObjectBuffer(GLfloat vertices[], GLfloat colors[]) {
		GLuint numVertices = 6;
		// Genderate 1 generic buffer object, called VBO
		GLuint VBO;
		glGenBuffers(1, &VBO);
		// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
		// Buffer will contain an array of vertices 
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
		glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);
		return VBO;
	}

	void linkCurrentBuffertoShader(GLuint shaderProgramID) {
		GLuint numVertices = 6;
		// find the location of the variables that we will be using in the shader program
		GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
		GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
		// Have to enable this
		glEnableVertexAttribArray(positionID);
		// Tell it where to find the position data in the currently active buffer (at index positionID)
		glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// Similarly, for the color data.
		glEnableVertexAttribArray(colorID);
		glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));
	}

public:
	Object(std::string objectPath)
		: VBO(0), NumVertices(0), NumTriangles(0), shader(0)
	{
		//Take the object file and parse it into a vbo
	}

	void AssignShader(Shader* Shader) 
	{
		this->shader = Shader;
	}

	void Draw()
	{
		shader->UseShader();
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
};