
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
#include "Shader.h"
#include "Object.h"
#include <vector>
#include "Model.h"
#include <random>
#include "Camera.h"
#include "FixedCamera.h"
#include "Player.h"
#include "Ghost.h"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define Width 800
#define Height 600

using namespace std;

Shader* playerShader;
Shader* ghostPanicShader;
Shader* ghostNormalShader;
vector<Object> myObjects;
vector<Mesh> meshes;

ICamera* activeCamera;

Camera* camera1;
Camera* camera2;
FixedCamera* camera3;

int onCamera = 1;

Model* orbit;
Ghost* ghost;
vector<Model> myModels;

Player* player;

GLfloat RotateZ = 0.0f;
GLfloat RotateY = 0.0f;
GLfloat RotateX = 0.0f;



// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{

	GLfloat translateScale = 0.5f;
	switch (key) {

	case'w':
		//activeCamera->TranslateZ(-translateScale);
		player->MoveUp(translateScale);
		break;
	case's':
		//activeCamera->TranslateZ(translateScale);
		player->MoveDown(translateScale);
		break;
	case'a':
		//activeCamera->TranslateX(-translateScale);
		player->MoveLeft(translateScale);
		break;
	case'd':
		//activeCamera->TranslateX(translateScale);
		player->MoveRight(translateScale);
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
	case 'q':
		activeCamera->RotateYaw(-10.0f);
		break;
	case 'e':
		activeCamera->RotateYaw(10.0f);
		break;
	case '1':
		activeCamera = camera1;
		break;
	case '2':
		activeCamera = camera2;
		break;
	case '3':
		activeCamera = camera3;
		break;
	case '0':
		ghost->GetModel()->SetShader(ghostPanicShader);
		ghost->SetMode(Panic);
		ghost->SetMovespeed(Ghost::SlowMoveSpeed());
		break;
	case '-':
		ghost->GetModel()->SetShader(ghostNormalShader);
		ghost->SetMode(Attack);
		ghost->SetMovespeed(Ghost::FastMoveSpeed());
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto timeValue = glutGet(GLUT_ELAPSED_TIME);



	glm::mat4 view = activeCamera->GetViewTransform();
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


	ghostNormalShader->SetUniform1f("time", timeValue);
	ghostNormalShader->SetUniform1f("rand", r);
	ghostNormalShader->SetUniformMatrix4fv("view", &view);
	ghostNormalShader->SetUniformMatrix4fv("projection", &projection);


	playerShader->SetUniform1f("time", timeValue);
	playerShader->SetUniform1f("rand", r);
	playerShader->SetUniformMatrix4fv("view", &view);
	playerShader->SetUniformMatrix4fv("projection", &projection);

	ghostPanicShader->SetUniform1f("time", timeValue);
	ghostPanicShader->SetUniform1f("rand", r);
	ghostPanicShader->SetUniformMatrix4fv("view", &view);
	ghostPanicShader->SetUniformMatrix4fv("projection", &projection);


	ghost->Move(player);
	ghost->Draw();
	player->Draw();


	for (int i = 0; i < myModels.size(); i++)
	{
		myModels.at(i).Draw();
	}

	glutPostRedisplay();
    glutSwapBuffers();
}


void init()
{
	glEnable(GL_DEPTH_TEST);

	camera1 = new Camera(glm::vec3(0.0f,3.0f,3.0f));
	camera2 = new Camera(glm::vec3(0.0f, 10.0f, 10.0f));
	// Camera with arial view looking straight down at the origin
	camera3 = new FixedCamera(glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	activeCamera = camera1;

	// Set up the shaders
	playerShader = new Shader("./playerVS.txt", "./playerFS.txt");
	ghostNormalShader = new Shader("./ghostNormalVS.txt", "./ghostNormalFS.txt");
	ghostPanicShader = new Shader("./ghostPanicVS.txt", "./ghostPanicFS.txt");



	auto timeValue = glutGet(GLUT_ELAPSED_TIME);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


	//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = activeCamera->GetViewTransform();
	projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);


	player = new Player(glm::vec3(5.0f, 0.0f, 0.0f), playerShader);
	ghost = new Ghost(glm::vec3(0.0f, 0.0f, 0.0f), ghostNormalShader);

	playerShader->SetUniform1f("rand", r);
	playerShader->SetUniform1f("time", timeValue);
	playerShader->SetUniformMatrix4fv("view", &view);
	playerShader->SetUniformMatrix4fv("projection", &projection);

	ghostNormalShader->SetUniform1f("rand", r);
	ghostNormalShader->SetUniform1f("time", timeValue);
	ghostNormalShader->SetUniformMatrix4fv("view", &view);
	ghostNormalShader->SetUniformMatrix4fv("projection", &projection);

	ghostPanicShader->SetUniform1f("rand", r);
	ghostPanicShader->SetUniform1f("time", timeValue);
	ghostPanicShader->SetUniformMatrix4fv("view", &view);
	ghostPanicShader->SetUniformMatrix4fv("projection", &projection);
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