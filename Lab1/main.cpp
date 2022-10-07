
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
#include "Object.cpp"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define Width 800
#define Height 600

using namespace std;

Shader* myShader;
Object* myDiamond;

GLfloat RotateZ = 0.0f;
GLfloat RotateY = 0.0f;
GLfloat RotateX = 0.0f;


// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{
	switch (key) {
		
	case 'q':
		RotateX -= 0.1f;
		break;
	case 'w':
		RotateX += 0.1f;
		break;
	case 'a':
		RotateY -= 0.1f;
		break;
	case 's':
		RotateY += 0.1f;
		break;
	case 'z':
		RotateZ -= 0.1f;
		break;
	case 'x':
		RotateZ += 0.1f;
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT);

	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	myShader->SetUniform1f("time", timeValue);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	model = 
		glm::rotate(glm::mat4(1.0f), RotateZ, glm::vec3(0.0f, 0.0f, 1.0f)) * 
		glm::rotate(glm::mat4(1.0f), RotateY, glm::vec3(0.0f, 1.0f, 0.0f)) * 
		glm::rotate(glm::mat4(1.0f), RotateX, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);



	myShader->SetUniformMatrix4fv("model", &model);
	myShader->SetUniformMatrix4fv("view", &view);
	myShader->SetUniformMatrix4fv("projection", &projection);

	glm::mat4 rotateZ = glm::mat4(1.0f);

	glutPostRedisplay();
	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	//glDrawArrays(GL_TRIANGLES, 0, 24);
	myDiamond->Draw();
    glutSwapBuffers();
}


void init()
{

	GLfloat diamondVerts[] = {
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		-1.0f,0.0f,1.0f,
		1.0,0.0f,1.0f,
		0.0f,1.0f,0.0f,

		1.0,0.0f,1.0f,
		1.0,0.0f,-1.0f,
		0.0f,1.0f,0.0f,

		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, -1.0f, 0.0f,

		-1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f,

		-1.0f,0.0f,1.0f,
		1.0,0.0f,1.0f,
		0.0f,-1.0f,0.0f,

		1.0,0.0f,1.0f,
		1.0,0.0f,-1.0f,
		0.0f,-1.0f,0.0f
	};

	GLfloat diamondColors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	// Set up the shaders
	myShader = new Shader("./vertexshader.txt", "./fragmentshader.txt", true);
	myDiamond = new Object(diamondVerts, diamondColors, 24, myShader);

	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	myShader->SetUniform1f("time", timeValue);


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

	myShader->SetUniformMatrix4fv("model", &model);
	myShader->SetUniformMatrix4fv("view", &view);
	myShader->SetUniformMatrix4fv("projection", &projection);
}



int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Hello Triangle"); 
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress); // allows for keyboard control. See keyPress function above

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