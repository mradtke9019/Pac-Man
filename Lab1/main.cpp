
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

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define Width 800
#define Height 600

using namespace std;

Shader* myShader;
Shader* assimpShader;
vector<Object> myObjects;
vector<Mesh> meshes;
Camera* camera;

vector<Model> myModels;

GLfloat RotateZ = 0.0f;
GLfloat RotateY = 0.0f;
GLfloat RotateX = 0.0f;

GLfloat CameraTranslateX = 0.0f;
GLfloat CameraTranslateY = 0.0f;
GLfloat CameraTranslateZ = 0.0f;


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "monkeyhead_smooth.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/


// function to allow keyboard control
// it's called a callback function and must be registerd in main() using glutKeyboardFunc();
// the functions must be of a specific format - see freeglut documentation
// similar functions exist for mouse control etc
void keyPress(unsigned char key, int x, int y)
{

	GLfloat translateScale = 0.5f;
	switch (key) {

	case'w':
		CameraTranslateZ -= translateScale;
		camera->CameraTranslateZ -= translateScale;
		break;
	case's':
		CameraTranslateZ += translateScale;
		camera->CameraTranslateZ += translateScale;
		break;
	case'a':
		CameraTranslateX -= translateScale;
		camera->CameraTranslateX -= translateScale;
		break;
	case'd':
		CameraTranslateX += translateScale;
		camera->CameraTranslateX += translateScale;
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
		camera->RotateYaw(-10.0f);
		break;
	case 'e':
		camera->RotateYaw(10.0f);
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

	glm::mat4 view = camera->GetViewTransform();
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	assimpShader->SetUniform1f("rand", r);
	assimpShader->SetUniformMatrix4fv("view", &view);
	assimpShader->SetUniformMatrix4fv("projection", &projection);

	myShader->SetUniformMatrix4fv("view", &view);
	myShader->SetUniformMatrix4fv("projection", &projection);


	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	//glDrawArrays(GL_TRIANGLES, 0, 24);
	for (int i = 0; i < myObjects.size(); i++) {
	glm::mat4 model = glm::mat4(1.0f);
		model =
			glm::rotate(glm::mat4(1.0f), RotateZ, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), RotateY, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), RotateX, glm::vec3(1.0f, 0.0f, 0.0f));
		myShader->SetUniformMatrix4fv("view", &view);
		myShader->SetUniformMatrix4fv("projection", &projection);
		myObjects.at(i).SetModelTransform(myObjects.at(i).GetModelTransform() * model);
		myObjects.at(i).Draw();
	}


	glutPostRedisplay();

	for (int i = 0; i < meshes.size(); i++) 
	{
		meshes.at(i).Draw();
	}
	for (int i = 0; i < myModels.size(); i++)
	{
		myModels.at(i).Draw();
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

	camera = new Camera();

	// Set up the shaders	// Set up the shaders
	assimpShader = new Shader("./assimpVertexShader.txt", "./assimpFragmentShader.txt", true);
	myShader = new Shader("./vertexshader.txt", "./fragmentshader.txt", true);



	myObjects.push_back(Object(diamondVerts, diamondColors, 24, myShader, glm::vec3(-2.0f, 0.0f, 0.0f)));
	myObjects.push_back(Object(diamondVerts, diamondColors, 24, myShader, glm::vec3(2.0f, 0.0f, 0.0f)));



	//Object meshObject = Object("./monkeyhead_smooth.dae", assimpShader);
	Model tree = Model("./Lowpoly_tree_sample.obj", assimpShader);
	tree.SetModelTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
	myModels.push_back(tree);
	

	//Model knight = Model("./knight.obj", assimpShader);
	//tree.SetModelTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
	//myModels.push_back(knight);

	Vertex v1 = Vertex();
	v1.Position = glm::vec3(-1.0f, 0.0f, 0.0f);
	Vertex v2 = Vertex();
	v2.Position = glm::vec3(1.0f, 0.0f, 0.0f);
	Vertex v3 = Vertex();
	v3.Position = glm::vec3(0.0f, 1.0f, 0.0f);
	std::vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	Mesh myMesh = Mesh(vertices, indices, (std::vector<Texture>)0, assimpShader);
	meshes.push_back(myMesh);

	auto timeValue = glutGet(GLUT_ELAPSED_TIME);
	myShader->SetUniform1f("time", timeValue);


	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	assimpShader->SetUniform1f("rand", r);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	camera->GetViewTransform();
	view = camera->GetViewTransform();
	projection = glm::perspective(glm::radians(60.0f), (float)Width / (float)Height, 0.1f, 100.0f);

	myShader->SetUniformMatrix4fv("view", &view);
	myShader->SetUniformMatrix4fv("projection", &projection);


	assimpShader->SetUniformMatrix4fv("view", &view);
	assimpShader->SetUniformMatrix4fv("projection", &projection);
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