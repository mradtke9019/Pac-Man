#include "Shader.cpp"
#include "Mesh.cpp"
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

#pragma once
class Object
{
private:
	Shader* shader;
	GLuint VBO;
	int NumVertices;
	int NumTriangles;
	glm::mat4 modelTransform;
	glm::vec3 position;

};