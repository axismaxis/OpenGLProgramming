#pragma once
#include<glm.hpp>
#include<Shader.h>
#include<Vertex.h>
#include<ObjModel.h>

class GameObject
{
public:
	GameObject(const char* objModelFileName, glm::vec3 position);
	~GameObject();

	void SetShader(Shader *shader);
	void Update(float fElapsedTime);
	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float fTotalTime);

	void ToggleRotate();

private:
	Shader* ObjectShader;
	ObjModel *mesh;

	glm::vec3 Position;
	bool rotating = true;
	float rotation = 0;
	float rotationSpeed = 0;
};