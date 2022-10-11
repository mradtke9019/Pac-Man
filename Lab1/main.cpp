
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
#include <vector>

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define Width 800
#define Height 600

using namespace std;

Shader* myShader;
vector<Object> myObjects;

GLfloat RotateZ = 0.0f;
GLfloat RotateY = 0.0f;
GLfloat RotateX = 0.0f;

GLfloat CameraTranslateX = 0.0f;
GLfloat CameraTranslateY = 0.0f;
GLfloat CameraTranslateZ = 0.0f;

// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{
	GLfloat translateScale = 0.1f;
	switch (key) {

	case'w':
		CameraTranslateZ -= translateScale;
		break;
	case's':
		CameraTranslateZ += translateScale;
		break;
	case'a':
		CameraTranslateX -= translateScale;
		break;
	case'd':
		CameraTranslateX += translateScale;
		break;
	case 'n':
		RotateX -= 0.1f;
		break;
	case 'm':
		RotateX += 0.1f;
		break;
	case 'k':
		RotateY -= 0.1f;
		break;
	case 'l':
		RotateY += 0.1f;
		break;
	case 'o':
		RotateZ -= 0.1f;
		break;
	case 'p':
		RotateZ += 0.1f;
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	myShader->SetUniform1f("time", timeValue);

	// The position of our camera in 3d space
	glm::vec3 cameraTranslation = glm::vec3(CameraTranslateX, CameraTranslateY, CameraTranslateZ);
	glm::vec3 cameraPos = glm::vec3(0.0f,3.0f,3.0f) + cameraTranslation;//glm::vec3(0.0f, 3.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f) + cameraTranslation;

	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);



	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	// glm::scale( X, vec3 ) = X * glm::scale( Identity, vec3 )
	model = 
		glm::rotate(glm::mat4(1.0f), RotateZ, glm::vec3(0.0f, 0.0f, 1.0f)) * 
		glm::rotate(glm::mat4(1.0f), RotateY, glm::vec3(0.0f, 1.0f, 0.0f)) * 
		glm::rotate(glm::mat4(1.0f), RotateX, glm::vec3(1.0f, 0.0f, 0.0f));
	

	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = glm::lookAt(cameraPos,
		cameraTarget,
		cameraUp);
	projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);



	//myShader->SetUniformMatrix4fv("model", &model);
	myShader->SetUniformMatrix4fv("view", &view);
	myShader->SetUniformMatrix4fv("projection", &projection);

	glm::mat4 rotateZ = glm::mat4(1.0f);

	glutPostRedisplay();
	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	//glDrawArrays(GL_TRIANGLES, 0, 24);
	for (int i = 0; i < myObjects.size(); i++) {
		myObjects.at(i).SetModelTransform(myObjects.at(i).GetModelTransform() * model);
		myObjects.at(i).Draw();
	}

    glutSwapBuffers();
}


void init()
{
	glEnable(GL_DEPTH_TEST);
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
	myObjects.push_back(Object(diamondVerts, diamondColors, 24, myShader, glm::vec3(-2.0f, 0.0f, 0.0f)));
	myObjects.push_back(Object(diamondVerts, diamondColors, 24, myShader, glm::vec3(2.0f, 0.0f, 0.0f)));
	

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
    glutCreateWindow("Matt Radtke - Graphics"); 
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