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
#pragma comment(lib, "glew32.lib")

MovementStates movementState = IDLE;

//Screen size
glm::ivec2 screenSize;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)screenSize.x / 2.0;
float lastY = (float)screenSize.y / 2.0;
bool firstMouse = true;

//Input manager
bool keysPressed[5000];

//Keep count of shaders
CubeMap *skybox;
Shader* cubemapShader;
std::vector<Shader*> shaders;
std::vector<Shader*>::const_iterator selectedShader;

//Timing
int lastTime = 0;

//Gameobjects
std::vector<GameObject*> gameObjects;

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);

	/*
		Create Shaders
	*/

	//Simple shader 
	Shader *simpleShader = new Shader("res/shaders/simple.vs", "res/shaders/simple.fs");

	//Texture shader 
	Shader *textureShader = new Shader("res/shaders/texture.vs", "res/shaders/texture.fs");

	//Texture lighted shader 
	Shader *textureLightedShader = new Shader("res/shaders/textureLighted.vs", "res/shaders/textureLighted.fs");

	//Toon shader
	Shader *toonShader = new Shader("res/shaders/toon.vs", "res/shaders/toon.fs");

	//Procedural texture shader
	Shader *proceduralShader = new Shader("res/shaders/procedural.vs", "res/shaders/procedural.fs");

	//Simplex noise shader
	Shader *noiseShader = new Shader("res/shaders/noise.vs", "res/shaders/noise.fs");

	//Bump map shader
	Shader *bumpmapShader = new Shader("res/shaders/bumpmap.vs", "res/shaders/bumpmap.fs");

	//Multitexture Shader
	Shader *multitextureShader = new Shader("res/shaders/multitexture.vs", "res/shaders/multitexture.fs");

	//Reflection Shader
	Shader *reflectionShader = new Shader("res/shaders/reflection.vs", "res/shaders/reflection.fs");

	//Reflection Shader
	Shader *refractionShader = new Shader("res/shaders/reflection.vs", "res/shaders/refraction.fs");

	//Add shaders to list
	shaders.push_back(refractionShader);
	shaders.push_back(reflectionShader);
	shaders.push_back(bumpmapShader);
	shaders.push_back(textureLightedShader);
	shaders.push_back(multitextureShader);
	shaders.push_back(simpleShader);
	shaders.push_back(textureShader);
	shaders.push_back(toonShader);
	shaders.push_back(proceduralShader);
	shaders.push_back(noiseShader);

	//Set default uniforms that i want in all shaders
	for (Shader *shader : shaders)
	{
		shader->EnableDebug(true);

		shader->CreateUniform("modelMatrix");
		shader->CreateUniform("viewMatrix");
		shader->CreateUniform("projectionMatrix");
		shader->CreateUniform("normalMatrix");
		shader->CreateUniform("cameraPos");
		shader->CreateUniform("time");
		shader->CreateUniform("s_texture");
		shader->CreateUniform("normalMap");
		shader->CreateUniform("secondTexture");
	}

	//Set iterator to point at first element
	selectedShader = shaders.begin();

	//Create skybox cubemap
	skybox = new CubeMap(	"res/textures/skybox1/right.jpg",
							"res/textures/skybox1/left.jpg",
							"res/textures/skybox1/top.jpg",
							"res/textures/skybox1/bottom.jpg",
							"res/textures/skybox1/back.jpg",
							"res/textures/skybox1/front.jpg");

	//CubeMap shader
	cubemapShader = new Shader("res/shaders/cubemap.vs", "res/shaders/cubemap.fs");
	cubemapShader->CreateUniform("viewMatrix");
	cubemapShader->CreateUniform("projectionMatrix");
	cubemapShader->CreateUniform("skybox");

	//Create gameobjects
	//GameObject* cube = new GameObject("res/models/cube/cube-textures.obj", glm::vec3(-0.5f, -0.5f, -0.0f));
	//cube->SetShader(*selectedShader);

	GameObject* car = new GameObject("res/models/car/honda_jazz.obj", glm::vec3(40.0f, -40.0f, -150.0f));
	car->SetShader(*selectedShader);

	//gameObjects.push_back(cube);
	gameObjects.push_back(car);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	//glutSetCursor(GLUT_CURSOR_NONE);
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int time = glutGet(GLUT_ELAPSED_TIME);

	// Create Model view projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 200.0f);		
	glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 view = camera.GetViewMatrix();

	//Draw objects
	for (GameObject *go : gameObjects)
	{
		go->Draw(view, projection, &camera, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	}

	//Draw skybox cubemap
	glDepthFunc(GL_LEQUAL);
	cubemapShader->Use();
	cubemapShader->SetUniformMatrix4fv("viewMatrix", skyboxView);
	cubemapShader->SetUniformMatrix4fv("projectionMatrix", projection);
	cubemapShader->SetUniformTexture("skybox", 0);
	glBindVertexArray(skybox->vaoID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	

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
	case 'e':
		selectedShader++;
		if (selectedShader == shaders.end())
		{
			selectedShader = shaders.begin();
		}
		for (GameObject *go : gameObjects)
		{
			go->SetShader(*selectedShader);
		}
		break;
	case 'q':
		if (selectedShader == shaders.begin())
		{
			selectedShader = shaders.end() - 1;
		}
		else
		{
			selectedShader--;
		}
		for (GameObject *go : gameObjects)
		{
			go->SetShader(*selectedShader);
		}
		break;
	case VK_SPACE:
		for (GameObject *go : gameObjects)
		{
			go->ToggleRotate();
		}
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

	for (GameObject *go : gameObjects)
	{
		go->Update(deltaTime);
	}

	glutPostRedisplay();
	lastTime = time;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1900, 1000);
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