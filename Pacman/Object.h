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

typedef struct ModelData
{
	size_t mPointCount = 0;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextureCoords;
};


class Object
{
private:
	Shader* shader;
	GLuint VBO;
	GLuint VAO;
	int NumVertices;
	int NumTriangles;
	glm::mat4 modelTransform;
	glm::vec3 position;

	void generateObjectBuffer(GLfloat vertices[], GLfloat colors[]) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		// Genderate 1 generic buffer object, called VBO
		glGenBuffers(1, &VBO);
		// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
		// Buffer will contain an array of vertices 
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, NumVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, NumVertices * 3 * sizeof(GLfloat), vertices);
		glBufferSubData(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(GLfloat), NumVertices * 4 * sizeof(GLfloat), colors);
	}

	void generateObjectBuffer(glm::vec3 vertices[], glm::vec4 colors[])
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
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
	}


	ModelData load_mesh(const char* file_name) {
		ModelData modelData;

		/* Use assimp to read the model file, forcing it to be read as    */
		/* triangles. The second flag (aiProcess_PreTransformVertices) is */
		/* relevant if there are multiple meshes in the model file that   */
		/* are offset from the origin. This is pre-transform them so      */
		/* they're in the right position.                                 */
		const aiScene* scene = aiImportFile(
			file_name,
			aiProcess_Triangulate | aiProcess_PreTransformVertices
		);

		if (!scene) {
			fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
			return modelData;
		}

		printf("  %i materials\n", scene->mNumMaterials);
		printf("  %i meshes\n", scene->mNumMeshes);
		printf("  %i textures\n", scene->mNumTextures);

		for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
			const aiMesh* mesh = scene->mMeshes[m_i];
			printf("    %i vertices in mesh\n", mesh->mNumVertices);
			modelData.mPointCount += mesh->mNumVertices;
			for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
				if (mesh->HasPositions()) {
					const aiVector3D* vp = &(mesh->mVertices[v_i]);
					modelData.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
				}
				if (mesh->HasNormals()) {
					const aiVector3D* vn = &(mesh->mNormals[v_i]);
					modelData.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
				}
				if (mesh->HasTextureCoords(0)) {
					const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
					modelData.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));
				}
				if (mesh->HasTangentsAndBitangents()) {
					/* You can extract tangents and bitangents here              */
					/* Note that you might need to make Assimp generate this     */
					/* data for you. Take a look at the flags that aiImportFile  */
					/* can take.                                                 */
				}
			}
		}

		aiReleaseImport(scene);
		return modelData;
	}

	void generateObjectBufferMesh(std::string path) {
		//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
		//Might be an idea to do a check for that before generating and binding the buffer.
		glGenVertexArrays(1, &VAO);
		ModelData mesh_data = load_mesh(path.c_str());
		unsigned int vp_vbo = 0;
		GLuint vPosition = shader->GetAttribLocation("vPosition");
		GLuint vNormal = shader->GetAttribLocation("vNormal");
		GLuint vTexture = shader->GetAttribLocation("vTexture");

		glGenBuffers(1, &vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(glm::vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
		
		unsigned int vn_vbo = 0;
		glGenBuffers(1, &vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(glm::vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

		//	This is for texture coordinates which you don't currently need, so I have commented it out
		//	unsigned int vt_vbo = 0;
		//	glGenBuffers (1, &vt_vbo);
		//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
		//	glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);

		glBindVertexArray(VAO);

		glEnableVertexAttribArray(vPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vNormal);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//	This is for texture coordinates which you don't currently need, so I have commented it out
		//	glEnableVertexAttribArray (loc3);
		//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
		//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
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

	Object(std::string objectPath, Shader* Shader);

	Shader* GetShader();

	void AssignShader(Shader* Shader);

	glm::mat4 GetModelTransform();

	void SetModelTransform(glm::mat4 Model);

	void Draw();
};