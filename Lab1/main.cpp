
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.cpp"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

Shader* myShader;

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
GLuint generateObjectBuffer(GLfloat vertices[], GLfloat colors[]) {
	GLuint numVertices = 6;
	// Genderate 1 generic buffer object, called VBO
	GLuint VBO;
 	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData (GL_ARRAY_BUFFER, 0, numVertices*3*sizeof(GLfloat), vertices);
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);
return VBO;
}

void linkCurrentBuffertoShader(GLuint shaderProgramID){
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
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*3*sizeof(GLfloat)));
}
#pragma endregion VBO_FUNCTIONS

void AddTriangles(GLfloat vertices[], GLuint shaderProgramID, int numTriangles, GLuint numVertices) {
	// Genderate 1 generic buffer object, called VBO
	GLuint* VBOs = new GLuint[numTriangles];
	unsigned int* VAOs = new GLuint[numTriangles];
	glGenVertexArrays(numTriangles, VAOs);
	glGenBuffers(numTriangles, VBOs);
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");

	for (int i = 0; i < numTriangles; i++) {
		glBindVertexArray(VAOs[i]);

		// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
		// Buffer will contain an array of vertices 
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);

	}

	int xyzSize = 3;
	int rgbSize = 3;
	int stride = sizeof(GLfloat) * xyzSize;// sizeof(GLfloat)* (xyzSize + rgbSize);

	// Have to enable this
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glEnableVertexAttribArray(positionID);
	glVertexAttribPointer(positionID, xyzSize, GL_FLOAT, GL_FALSE, stride, 0);

	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, rgbSize, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));

	glUseProgram(shaderProgramID);
}

void LearnOpenGLTest(GLfloat vertices[], GLuint shaderProgramID, int numTriangles, GLuint numVertices)
{
	unsigned int vao;
	int vaoCount = 1;
	glGenVertexArrays(vaoCount, &vao);
	glBindVertexArray(vao);

	// Want 1 vbo for our triangles
	int vboCount = 1;
	GLuint vbo;
	// Create, bind, load
	glGenBuffers(vboCount, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Mode (GL_ARRAY_BUFFER), 
	//size - specify the size in bytes of the buffer objects new data,
	//data - specifies a pointer to the data that will be copied into the buffer,
	// usage - specify the usage pattern of the data (static, dynamic, stream)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);
	// Find out where our position is placed in the shader
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	int totalItemsPerVertex = 3;
	int stride = totalItemsPerVertex * sizeof(GLfloat);
	 // Position of shader input
	// size, how loarge our vertex data is, usually 3 for xyz but can have more
	// type of data for this attribute ,FLOAT
	// normalize? determine if we want to normalize our data, usually no
	// stride - the space in byutes that differentiates data in 1 vertex vs another
	// offset - where do we start in our vertex data, usually 0 
	glVertexAttribPointer(positionID, totalItemsPerVertex, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(positionID); // enable the attribute in the shader

	glBindVertexArray(vao);
	glUseProgram(shaderProgramID);

}

std::vector<string> split(string text, string delimiter) {
	vector<string> words{};

	size_t pos = 0;
	while ((pos = text.find(delimiter)) != string::npos) {
		words.push_back(text.substr(0, pos));
		text.erase(0, pos + delimiter.length());
	}
	for (const auto& str : words) {
		cout << str << endl;
	}
	return words;
}

GLuint** getTriangleData() {

	const char* file = "./triangles.txt";

	std::ifstream dataStream(file);
	std::string data((std::istreambuf_iterator<char>(dataStream)),
		std::istreambuf_iterator<char>());

	std::ifstream ifs(file);
	std::ifstream filestream(file);

	std::string line;
	int i = 0;
	int triangleCount = 0;
	while (std::getline(ifs, line))
	{
		if (i % 3 == 0) 
		{
			triangleCount++;
		}
		i++;
	}

	while (std::getline(filestream, line))
	{
		std::vector<string> vertexData = split(line, " ");
		for (const auto& str : vertexData) {
			cout << str << endl;
		}
	}

	GLuint** triangleData = new GLuint * [triangleCount];
	for (i = 0; i < triangleCount; i++)
	{

	}

	return NULL;
}

void CustomOpenGLLearningCode() {

	// Get our vertex buffer objects to store vertex data in gpu
	unsigned int VBOs[5];
	glGenBuffers(5, VBOs);
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	for (int i = 0; i < 5; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

}


void display(){

	glClear(GL_COLOR_BUFFER_BIT);
	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	int timeLocation = glGetUniformLocation(myShader->GetShaderProgramID(), "time");
	glUniform1f(timeLocation, timeValue);
	glutPostRedisplay();
	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	glDrawArrays(GL_TRIANGLES, 0, 6);
    glutSwapBuffers();
}


void init()
{
	GLfloat allVerticesWithColor[] = {
		// x,y,z			r,g,b
		-1.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0,
		0.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0,
		-0.5f, 1.0f, 0.0f, 1.0, 0.0, 0.0,
		0.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0,
		1.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0,
		0.5f, 1.0f, 0.0f, 1.0, 0.0, 0.0,
	};

	GLfloat allVertices[] = {
		// x,y,z			
		- 1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};

	// Create 3 vertices that make up a triangle that fits on the viewport 
	GLfloat vertices[] = {
			-1.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			-0.5f, 1.0f, 0.0f
	};

	GLfloat vertices2[] = {
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};

	//getTriangleData();


	//int length = mySize<GLfloat*>(triangles);

	// Set up the shaders
	myShader = new Shader("./vertexshader.txt", "./fragmentshader.txt", true);
	//shaderProgramID1 = CompileShaders("./vertexshader.txt", "./fragmentshader.txt");
	//GLuint shaderProgramID2 = CompileShaders("./vertexshader2.txt", "./fragmentshader2.txt");
	// Put the vertices and colors into a vertex buffer object
	//generateObjectBuffer(vertices, colors);
	// Link the current buffer to the shader
	//linkCurrentBuffertoShader(shaderProgramID);
	
	// add time to shader
	//glUniform1f(time, timeValue);


	AddTriangles(allVertices, myShader->GetShaderProgramID(), 2, 6);
	int numTriangles = 2;
	int vertexCount = numTriangles * 3;

	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	myShader->SetUniform1f("time", timeValue);
	

	glm::mat4x4 MVP;
	glm::mat4x4 identity;

	int i, j;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)
			{ 
				identity[i][j] = 1;
				MVP[i][j] = 1;
			}
			else
			{
				identity[i][j] = 0;
				MVP[i][j] = 0;
			}
			printf("%f, ", identity[i][j]);
		}
		printf("\n");
	}
	MVP[0][3] = 0.5;
	MVP[1][3] = 0.5;
	MVP[2][3] = 0.5;


	//GLuint MatrixID = glGetUniformLocation(myShader->GetShaderProgramID(), "MVP");
	//GLuint MatrixIdentity = glGetUniformLocation(myShader->GetShaderProgramID(), "Identity");
	//// Send our transformation to the currently bound shader,
	//// in the "MVP" uniform
	//// For each model you render, since the MVP will be different (at least the M part)
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//glUniformMatrix4fv(MatrixIdentity, 1, GL_FALSE, &identity[0][0]);

	myShader->SetUniformMatrix4fv("MVP", &MVP);
	myShader->SetUniformMatrix4fv("Identity", &identity);

	//LearnOpenGLTest(vertices, shaderProgramID1, 1, 3);
	//LearnOpenGLTest(allVertices, shaderProgramID1, numTriangles, vertexCount);

	//AddTriangles(vertices, colors, shaderProgramID1, 1, 3);
	//AddTriangles(vertices2, colors, shaderProgramID2, 1, 3);
}



int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello Triangle");
	// Tell glut where the display function is
	glutDisplayFunc(display);

	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}