#pragma once
#include<glm.hpp>
#include<Shader.h>
#include<Vertex.h>

class TexturedObject
{
public:
	TexturedObject(glm::vec3 position);
	~TexturedObject();

	void SetShader(Shader *shader);
	void Update(float fElapsedTime);
	void Draw(glm::mat4 viewProjectionMatrix);

private:
	TexturedVertex *vertices = nullptr;
	Shader* ObjectShader;

	glm::vec3 Position;
	float rotation = 0;
	float rotationSpeed = 0;

	void GenerateMesh();
};