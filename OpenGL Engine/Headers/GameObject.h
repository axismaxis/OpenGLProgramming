#pragma once
#include<glm.hpp>
#include<Shader.h>
#include<Vertex.h>

class GameObject
{
public:
	GameObject(glm::vec3 position);
	~GameObject();

	void SetShader(Shader *shader);
	void Update(float fElapsedTime);
	void Draw(glm::mat4 viewProjectionMatrix);

	void AddRandomTriangle();
	void RemoveRandomTriangle();
	void SetDisco(bool enable);
	void ToggleDisco();
private:
	Vertex *vertices = nullptr;
	Shader* ObjectShader;

	glm::vec3 Position;
	float rotation = 0;
	float rotationSpeed = 0;
	bool disco = false;

	int vertexAmount = 9;
	
	void GenerateRandomVertices();
};