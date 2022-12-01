
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
#include "Arena.h"
#include <irrKlang.h>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
int Width;
int Height;

using namespace std;
using namespace irrklang;

Shader* ghostPanicShader;
Shader* commonShader;
ISoundEngine* SoundEngine;

ICamera* activeCamera;

FixedCamera defaultCamera;
FixedCamera tiltedCamera;

bool Pause;

vector<Ghost*> ghosts;
vector<Model> myModels;
vector<Model> particles;

Arena* arena;

Player* player;

// Idea to use delta frame from here https://learnopengl.com/Getting-started/Camera
float deltaTime;// Time between current frame and last frame
float lastFrame; // Time of last frame
float currentFrame;

glm::vec3 LightColor;
glm::vec3 LightPosition; 
glm::vec3 LightDirection;


glm::mat4 GetProjection()
{
	return glm::perspective(glm::radians(60.0f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 300.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	currentFrame = timeValue;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	glm::mat4 view = activeCamera->GetViewTransform();
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	glm::mat4 projection = GetProjection();

	commonShader->SetUniformVec3("LightColor", LightColor);
	commonShader->SetUniformVec3("LightPosition", LightPosition);
	commonShader->SetUniformVec3("LightDirection", LightDirection);
	commonShader->SetUniform1f("time", timeValue);
	commonShader->SetUniform1f("rand", r);
	commonShader->SetUniformMatrix4fv("view", &view);
	commonShader->SetUniformMatrix4fv("projection", &projection);
	
	ghostPanicShader->SetUniform1f("time", timeValue);
	ghostPanicShader->SetUniform1f("rand", r);
	ghostPanicShader->SetUniformMatrix4fv("view", &view);
	ghostPanicShader->SetUniformMatrix4fv("projection", &projection);



	player->SetDeltaTime(deltaTime);
	if (!Pause)
	{
		player->Move(arena);
		for (int i = 0; i < ghosts.size(); i++)
		{
			ghosts.at(i)->SetDeltaTime(deltaTime);
			ghosts.at(i)->Move(player, arena);
			if (arena->Collision(player->GetPosition(), ghosts.at(i)->GetPosition()))
			{
				Pause = true;
				SoundEngine->play2D("./Pacman-death-sound.mp3");
			}
		}
		for (int i = 0; i < arena->GetPoints()->size(); i++)
		{
			if (arena->Collision(player->GetPosition(), arena->GetPoints()->at(i)->GetPosition()))
			{
				arena->GetPoints()->erase(arena->GetPoints()->begin() + i);
				if (arena->GetPoints()->size() == 0 && arena->GetFruits()->size() == 0) 
				{
					Pause = true;
					SoundEngine->play2D("./mixkit-winning-an-extra-bonus-2060.wav");
				}
				else SoundEngine->play2D("./mixkit-small-hit-in-a-game-2072.wav");
				break;
			}
		}
		for (int i = 0; i < arena->GetFruits()->size(); i++)
		{
			if (arena->Collision(player->GetPosition(), arena->GetFruits()->at(i)->GetPosition()))
			{
				arena->GetFruits()->erase(arena->GetFruits()->begin() + i);
				if (arena->GetPoints()->size() == 0 && arena->GetFruits()->size() == 0)
				{
					Pause = true;
					SoundEngine->play2D("./mixkit-winning-an-extra-bonus-2060.wav");
				}
				else
					SoundEngine->play2D("./mixkit-small-hit-in-a-game-2072.wav");
				break;
			}
		}

	}
	arena->Draw();
	for (int i = 0; i < ghosts.size(); i++)
	{
		ghosts.at(i)->Draw();
	}

	player->Draw();


	float angleStep = 360.0f / particles.size();
	glm::mat4 playerModel = glm::translate(glm::mat4(1.0f), player->GetPosition());
	for (int i = 0; i < particles.size(); i++) {
		float angle = angleStep * i;
		glm::mat4 particleTransform =
			playerModel *
			glm::rotate(glm::mat4(1.0f), glm::radians(angle * timeValue * 0.005f), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));

		particles.at(i).SetModelTransform(particleTransform);
		particles.at(i).Draw();
	}

	glutPostRedisplay();
    glutSwapBuffers();
}

void LoadShaders()
{
	// Set up the shaders
	ghostPanicShader = new Shader("./ghostPanicVS.txt", "./ghostPanicFS.txt");
	commonShader = new Shader("./VS1.txt", "./FS1.txt");
}

void LoadArena()
{
	if (arena != nullptr)
	{
		delete arena;
		arena = nullptr;
	}
		
	arena = new Arena("./arena.txt", commonShader);
}

void LoadCameras()
{
	defaultCamera = FixedCamera(glm::vec3(0.0f, 150.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	tiltedCamera = FixedCamera(glm::vec3(150.0f, 150.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
}

void LoadObjects()
{
	player = new Player(glm::vec3(0,0,0), commonShader);
	ghosts = std::vector<Ghost*>();
	particles = std::vector<Model>();

	for (int i = 0; i < arena->GetGhostInitialPositions().size(); i++)
	{
		glm::vec3 randomColor = glm::vec3(
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		ghosts.push_back(new Ghost(glm::vec3(0,0,0), commonShader, i * 5, randomColor));
	}

	for (int i = 0; i < 6; i++) {
		particles.push_back(Model("./point.obj", player->GetPosition(), commonShader, glm::vec3(1.0, 0.0, 1.0)));
	}
	SoundEngine = createIrrKlangDevice();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	//Color initialzations
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	Pause = false;

	activeCamera = &defaultCamera;


	player->SetPosition(arena->GetPlayerInitialPosition());
	player->SetDirection(None);
	for (int i = 0; i < ghosts.size(); i++)
	{
		ghosts.at(i)->SetPosition(arena->GetGhostInitialPositions().at(i));
	}


	LightColor = glm::vec3(0.5, 0.5, 0.5);
	LightPosition = glm::vec3(-1.0, 1.0, -0.3);
	LightDirection = glm::vec3(0.1, -1.0, -0.3);

	auto timeValue = glutGet(GLUT_ELAPSED_TIME);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = activeCamera->GetViewTransform();
	projection = GetProjection();

	commonShader->SetUniform1f("rand", r);
	commonShader->SetUniform1f("time", timeValue);
	commonShader->SetUniformMatrix4fv("view", &view);
	commonShader->SetUniformMatrix4fv("projection", &projection);
	commonShader->SetUniformVec3("LightColor", LightColor);
	commonShader->SetUniformVec3("LightPosition", LightPosition);
	commonShader->SetUniformVec3("LightDirection", LightDirection);
	
	ghostPanicShader->SetUniform1f("rand", r);
	ghostPanicShader->SetUniform1f("time", timeValue);
	ghostPanicShader->SetUniformMatrix4fv("view", &view);
	ghostPanicShader->SetUniformMatrix4fv("projection", &projection);

	SoundEngine->play2D("./intro.wav");
}


// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{
	switch (key) {
	case' ':
		Pause = !Pause;
		break;
	case'w':
		//activeCamera->TranslateZ(-translateScale);
		player->SetDirection(Up);
		break;
	case's':
		//activeCamera->TranslateZ(translateScale);
		player->SetDirection(Down);
		break;
	case'a':
		//activeCamera->TranslateX(-translateScale);
		player->SetDirection(Left);
		break;
	case'd':
		//activeCamera->TranslateX(translateScale);
		player->SetDirection(Right);
		break;
	case 'r':
		LoadArena();
		init();
	case '1':
		activeCamera = &defaultCamera;
		break;
	case '2':
		activeCamera = &tiltedCamera;
		break;
	case '3':
		break;
	case '0':
		for (int i = 0; i < ghosts.size(); i++)
		{
			ghosts.at(i)->GetModel()->SetShader(ghostPanicShader);
			ghosts.at(i)->SetMode(Panic);
			ghosts.at(i)->SetMovespeed(Ghost::SlowMoveSpeed());
		}
		break;
	case '-':
		for (int i = 0; i < ghosts.size(); i++)
		{
			ghosts.at(i)->GetModel()->SetShader(commonShader);
			ghosts.at(i)->SetMode(Attack);
			ghosts.at(i)->SetMovespeed(Ghost::FastMoveSpeed());
		}
		break;
	case ';':
		LightDirection.z -= 1.0f;
		break;
	case '\'':
		LightDirection.z += 1.0f;
		break;
	case ',':
		LightDirection.x -= 1.0f;
		break;
	case '.':
		LightDirection.x += 1.0f;
		break;
	}

	// we must call these to redraw the scene after we make any changes 
	glutPostRedisplay();
}

int main(int argc, char** argv){
	Width = 800;
	Height = 600;
	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Pacman"); 
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress); // allows for keyboard control. See keyPress function above
		// set reshape callback for current window
		
	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	LoadShaders();
	LoadArena();
	LoadCameras();
	LoadObjects();
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}