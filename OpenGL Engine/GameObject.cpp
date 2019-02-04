#include<GameObject.h>
#include<GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <time.h>

GameObject::GameObject(glm::vec3 position)
{
	Position = position;
	GenerateRandomVertices();
	rotationSpeed = std::rand() % 100 / 10.0f;
}

GameObject::~GameObject()
{
	free(ObjectShader);
}

void GameObject::SetShader(Shader *shader)
{
	ObjectShader = shader;
}

void GameObject::Update(float fElapsedTime)
{
	rotation += fElapsedTime / 1000.0f * rotationSpeed;
}

void GameObject::SetDisco(bool enable)
{
	disco = enable;
}

void GameObject::ToggleDisco()
{
	disco = !disco;
}

void GameObject::Draw(glm::mat4 viewProjectionMatrix)
{
	//Only do these in the object
	glm::mat4 mvp = glm::translate(viewProjectionMatrix, glm::vec3(0, -2, -2));
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 1, 0));

	// Set MVP in shader uniform
	ObjectShader->Use();
	ObjectShader->SetUniformMatrix4fv("modelViewProjectionMatrix", mvp);
	ObjectShader->SetUniformFloat("time", 10.0f);

	if (disco)
	{
		for (int i = 0; i < vertexAmount; i++)
		{
			vertices[i].color.r = ((rand() % 1000) / 1000.0f);
			vertices[i].color.g = ((rand() % 1000) / 1000.0f);
			vertices[i].color.b = ((rand() % 1000) / 1000.0f);
		}
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), vertices);									//geef aan dat de posities op deze locatie zitten
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &vertices[0].color);					//geef aan dat de kleuren op deze locatie zitten
	glDrawArrays(GL_TRIANGLES, 0, vertexAmount);																//en tekenen :)
}

void GameObject::AddRandomTriangle()
{
	vertexAmount += 3;
	GenerateRandomVertices();
}

void GameObject::RemoveRandomTriangle()
{
	if (vertexAmount >= 6)
	{
		vertexAmount -= 3;
		GenerateRandomVertices();
	}
}

void GameObject::GenerateRandomVertices()
{
	if(vertices)
		free(vertices);

	vertices = (Vertex*)malloc(sizeof(Vertex) * vertexAmount);
	for (int i = 0; i < vertexAmount; i++)
	{
		glm::vec3 position = { 3.0f * ((rand() % 1000) / 1000.0f),
								3.0f * ((rand() % 1000) / 1000.0f),
								3.0f * ((rand() % 1000) / 1000.0f) };
		glm::vec4 color = { 1.0f * ((rand() % 1000) / 1000.0f),
							1.0f * ((rand() % 1000) / 1000.0f),
							1.0f * ((rand() % 1000) / 1000.0f),
							1.0f };
		vertices[i].position = position;
		vertices[i].color = color;
	}
}