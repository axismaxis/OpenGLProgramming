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

//Shaders
Shader* simpleShader;
Shader* grayscaleShader;

//Timing
int lastTime;

//Gameobjects
std::vector<GameObject*> gameObjects;

void checkShaderErrors(GLuint shaderId)
{
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);					//kijk of het compileren is gelukt
	if (status == GL_FALSE)
	{
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);				//haal de lengte van de foutmelding op
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);		//en haal de foutmelding zelf op
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}
}


#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);

	//Simple shader 
	simpleShader = new Shader("simple.vs", "simple.fs");
	simpleShader->EnableDebug(true);
	simpleShader->Link();

	simpleShader->CreateUniform("modelViewProjectionMatrix");
	simpleShader->CreateUniform("time");

	//Only grayscale colors
	grayscaleShader = new Shader("simple.vs", "grayscale.fs");
	grayscaleShader->EnableDebug(true);
	grayscaleShader->Link();

	grayscaleShader->CreateUniform("modelViewProjectionMatrix");
	grayscaleShader->CreateUniform("time");

	//Create gameobjects
	GameObject* cube = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f));
	cube->SetShader(simpleShader);

	gameObjects.push_back(cube);

	lastTime = glutGet(GLUT_ELAPSED_TIME);
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create Model view projection matrix
	glm::mat4 mvp = glm::perspective(80.0f, screenSize.x / (float)screenSize.y, 0.01f, 100.0f);		//begin met een perspective matrix
	mvp *= glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	
	//Draw objects
	for (GameObject *go : gameObjects)
	{
		go->Draw(mvp);
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
	if (key == VK_ESCAPE)
		glutLeaveMainLoop();

	if (key == 'f')
	{
		gameObjects[0]->AddRandomTriangle();
	}

	if (key == 's')
	{		
		gameObjects[0]->RemoveRandomTriangle();
	}

	if (key == 'd')
	{
		gameObjects[0]->ToggleDisco();
	}

	if (key == 'q')
	{
		gameObjects[0]->SetShader(simpleShader);
	}

	if (key == 'w')
	{
		gameObjects[0]->SetShader(grayscaleShader);
	}

	if (key == 'x')
	{
		GameObject* cube = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f));
		cube->SetShader(simpleShader);

		gameObjects.push_back(cube);
	}

	if (key == 'z')
	{
		if (gameObjects.size() > 1)
		{
			gameObjects.pop_back();
		}	
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