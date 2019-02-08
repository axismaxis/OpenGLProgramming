#pragma once
#include<glm.hpp>
#include<Shader.h>
#include<Vertex.h>
#include<ObjModel.h>

class GameObject
{
public:
	GameObject(glm::vec3 position);
	~GameObject();

	void SetShader(Shader *shader);
	void Update(float fElapsedTime);
	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float fTotalTime);

private:
	Shader* ObjectShader;
	ObjModel *mesh;

	glm::vec3 Position;
	float rotation = 0;
	float rotationSpeed = 0;
};