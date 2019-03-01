#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <Shader.h>
#include <Vertex.h>
#include <GameObject.h>
#include <vector>
#include "CubeMap.h"
#include "Camera.h"
#include "FBO.h"
#include "Model.h"
#pragma comment(lib, "glew32.lib")

//Screen size
glm::ivec2 screenSize;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)screenSize.x / 2.0f;
float lastY = (float)screenSize.y / 2.0f;
bool firstMouse = true;
MovementStates movementState = IDLE;

//Input manager
bool keysPressed[5000];

//Keep track of scene shaders
std::vector<Shader*> sceneShaders;
std::vector<Shader*>::const_iterator selectedSceneShader;

//Timing
int lastTime = 0;

Model* gameObject = new Cube();

void SetupSceneShaders()
{
	//Simple shader 
	Shader *simpleShader = new Shader("res/shaders/simple.vs", "res/shaders/simple.fs");

	//Add shaders to list
	sceneShaders.push_back(simpleShader);

	//Set default uniforms that i want in all shaders
	for (Shader *shader : sceneShaders)
	{
		shader->EnableDebug(true);

		shader->CreateUniform("modelMatrix");
		shader->CreateUniform("viewMatrix");
		shader->CreateUniform("projectionMatrix");
	}

	//Set iterator to point at first element
	selectedSceneShader = sceneShaders.begin();
}

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);

	gameObject->Load();
	SetupSceneShaders();
	
	//glutSetCursor(GLUT_CURSOR_NONE);
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int time = glutGet(GLUT_ELAPSED_TIME);

	// Create Model view projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 200.0f);		
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, -0.5f));

	//Bind framebuffer to render to
	glViewport(0, 0, 1900, 1000);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use shader
	(*selectedSceneShader)->Use();
	(*selectedSceneShader)->SetUniformMatrix4fv("viewMatrix", view);
	(*selectedSceneShader)->SetUniformMatrix4fv("projectionMatrix", projection);
	(*selectedSceneShader)->SetUniformMatrix4fv("modelMatrix", model);

	//Draw objects
	gameObject->Draw();
	
	//Swap buffer to screen
	glutSwapBuffers();
}

void reshape(int newWidth, int newHeight)
{
	screenSize.x = newWidth;
	screenSize.y = newHeight;
	glutPostRedisplay();
}

//Keydown events
void keyboard(unsigned char key, int x, int y)
{
	keysPressed[key] = true;

	switch (key)
	{
	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	case 'w':
		movementState = FORWARD;
		break;
	case 'a':
		movementState = LEFT;
		break;
	case 'd':
		movementState = RIGHT;
		break;
	case 's':
		movementState = BACKWARD;
		break;
	}
}

//Key up events
void keyboardUp(unsigned char key, int x, int y)
{
	keysPressed[key] = false;

	if (!keysPressed['w'] && !keysPressed['a'] && !keysPressed['s'] && !keysPressed['d'])
	{
		movementState = IDLE;
	}
}

void mouse(int x, int y)
{
	if (firstMouse)
	{
		lastX = (float)x;
		lastY = (float)y;
		firstMouse = false;
	}

	float xoffset = (float)x - lastX;
	float yoffset = lastY - (float)y; // reversed since y-coordinates go from bottom to top

	lastX = (float)x;
	lastY = (float)y;

	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	if (x < 10 || y > screenHeight - 10 || x > screenWidth - 10 || y < 10)
	{
		glutWarpPointer(screenSize.x / 2, screenSize.y / 2);
	}
	

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = time - lastTime;

	camera.ProcessKeyboard(movementState, deltaTime / 1000.0f);

	glutPostRedisplay();
	lastTime = time;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1900, 1000);
	screenSize.x = 1900;
	screenSize.y = 1000;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Visualisatietechnieken");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(update);

	init();
	
	glutMainLoop();
}