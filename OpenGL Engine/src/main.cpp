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
#pragma comment(lib, "glew32.lib")

//Screen size
glm::ivec2 screenSize;

//Keep count of shaders
std::vector<Shader*> shaders;
std::vector<Shader*>::const_iterator selectedShader;

//Timing
int lastTime;

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
	Shader *toonShader = new Shader("res/shaders/simple.vs", "res/shaders/toon.fs");

	//Add shaders to list
	shaders.push_back(simpleShader);
	shaders.push_back(textureShader);
	shaders.push_back(textureLightedShader);
	shaders.push_back(toonShader);

	//Set default uniforms that i want in all shaders
	for (Shader *shader : shaders)
	{
		shader->EnableDebug(true);

		shader->CreateUniform("modelMatrix");
		shader->CreateUniform("viewMatrix");
		shader->CreateUniform("projectionMatrix");
		shader->CreateUniform("normalMatrix");
		shader->CreateUniform("time");
		shader->CreateUniform("s_texture");
	}

	//Set iterator to point at first element
	selectedShader = shaders.begin();

	//Create gameobjects
	GameObject* cube = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f));
	cube->SetShader(*selectedShader);

	gameObjects.push_back(cube);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create Model view projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 200.0f);		
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 150), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					
	
	//Draw objects
	for (GameObject *go : gameObjects)
	{
		go->Draw(view, projection, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	}

	//Swap buffer to screen
	glutSwapBuffers();
}

void reshape(int newWidth, int newHeight)
{
	screenSize.x = newWidth;
	screenSize.y = newHeight;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		glutLeaveMainLoop();
		break;
	case 'w':
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
	}
}

void update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = time - lastTime;

	for (GameObject *go : gameObjects)
	{
		go->Update(elapsed);
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
	glutIdleFunc(update);

	init();
	
	glutMainLoop();
}