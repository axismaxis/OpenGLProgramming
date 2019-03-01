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

//Skybox variables
CubeMap *skybox;
Shader *cubemapShader;

//Keep track of scene shaders
std::vector<Shader*> sceneShaders;
std::vector<Shader*>::const_iterator selectedSceneShader;

//Keep track of post processing shaders
std::vector<Shader*> PPShaders;
std::vector<Shader*>::const_iterator selectedPPShader;

//Framebuffers
FBO *fbo1;

//Timing
int lastTime = 0;

//Gameobjects
std::vector<GameObject*> gameObjects;

void SetupSceneShaders()
{
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
	sceneShaders.push_back(refractionShader);
	sceneShaders.push_back(reflectionShader);
	sceneShaders.push_back(bumpmapShader);
	sceneShaders.push_back(textureLightedShader);
	sceneShaders.push_back(multitextureShader);
	sceneShaders.push_back(simpleShader);
	sceneShaders.push_back(textureShader);
	sceneShaders.push_back(toonShader);
	sceneShaders.push_back(proceduralShader);
	sceneShaders.push_back(noiseShader);

	//Set default uniforms that i want in all shaders
	for (Shader *shader : sceneShaders)
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
	selectedSceneShader = sceneShaders.begin();
}

void SetupSkybox()
{
	//Create skybox cubemap
	skybox = new CubeMap("res/textures/skybox1/right.jpg",
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
}

void SetupFBO()
{
	//Create FBO for later rendering
	//fbo1 = new FBO(3840, 2160);
	fbo1 = new FBO(1920, 1080);

	Shader *FBOSimpleShader = new Shader("res/shaders/FBOSimple.vs", "res/shaders/FBOSimple.fs");
	Shader *FBOEdgeShader = new Shader("res/shaders/FBOEdge.vs", "res/shaders/FBOEdge.fs");
	Shader *FBOWarpShader = new Shader("res/shaders/FBOWarp.vs", "res/shaders/FBOWarp.fs");
	Shader *FBOSwirlShader = new Shader("res/shaders/FBOSwirl.vs", "res/shaders/FBOSwirl.fs");
	Shader *FBOFishEyeShader = new Shader("res/shaders/FBOFishEye.vs", "res/shaders/FBOFishEye.fs");

	PPShaders.push_back(FBOSimpleShader);
	PPShaders.push_back(FBOEdgeShader);
	PPShaders.push_back(FBOWarpShader);
	PPShaders.push_back(FBOSwirlShader);
	PPShaders.push_back(FBOFishEyeShader);

	for (Shader *shader : PPShaders)
	{
		shader->EnableDebug(true);
		shader->CreateUniform("time");
		shader->CreateUniform("screenTexture");
	}

	selectedPPShader = PPShaders.begin();

	fbo1->SetShader(*selectedPPShader);
}

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);

	SetupSceneShaders();
	SetupSkybox();
	SetupFBO();
	
	//Create gameobjects
	//GameObject* cube = new GameObject("res/models/cube/cube-textures.obj", glm::vec3(-0.5f, -0.5f, -0.0f));
	//cube->SetShader(*selectedSceneShader);

	GameObject* car = new GameObject("res/models/car/honda_jazz.obj", glm::vec3(40.0f, -40.0f, -150.0f));
	car->SetShader(*selectedSceneShader);

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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int time = glutGet(GLUT_ELAPSED_TIME);

	// Create Model view projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 200.0f);		
	glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 view = camera.GetViewMatrix();

	//Bind framebuffer to render to
	fbo1->Bind();
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, fbo1->Width, fbo1->Height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	/*
		Done Rendering to FBO
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT);

	//Render framebuffer to quad
	fbo1->Render();
	
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
	case '1':
		if (selectedPPShader == PPShaders.begin())
		{
			selectedPPShader = PPShaders.end() - 1;
		}
		else
		{
			selectedPPShader--;
		}
		fbo1->SetShader(*selectedPPShader);
		break;
	case '3':
		selectedPPShader++;
		if (selectedPPShader == PPShaders.end())
		{
			selectedPPShader = PPShaders.begin();
		}
		fbo1->SetShader(*selectedPPShader);
		break;
	case 'e':
		selectedSceneShader++;
		if (selectedSceneShader == sceneShaders.end())
		{
			selectedSceneShader = sceneShaders.begin();
		}
		for (GameObject *go : gameObjects)
		{
			go->SetShader(*selectedSceneShader);
		}
		break;
	case 'q':
		if (selectedSceneShader == sceneShaders.begin())
		{
			selectedSceneShader = sceneShaders.end() - 1;
		}
		else
		{
			selectedSceneShader--;
		}
		for (GameObject *go : gameObjects)
		{
			go->SetShader(*selectedSceneShader);
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