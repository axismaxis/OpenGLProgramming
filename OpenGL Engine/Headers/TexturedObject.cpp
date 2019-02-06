#include<TexturedObject.h>
#include<GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <time.h>

TexturedObject::TexturedObject(glm::vec3 position)
{
	Position = position;
	GenerateMesh();
	rotationSpeed = std::rand() % 100 / 10.0f;
}

TexturedObject::~TexturedObject()
{
	free(ObjectShader);
}

void TexturedObject::SetShader(Shader *shader)
{
	ObjectShader = shader;
}

void TexturedObject::Update(float fElapsedTime)
{
	rotation += fElapsedTime / 1000.0f * rotationSpeed;
}

void TexturedObject::Draw(glm::mat4 viewProjectionMatrix)
{
	//Only do these in the object
	glm::mat4 mvp = glm::translate(viewProjectionMatrix, glm::vec3(0, -2, -2));
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 1, 0));

	// Set MVP in shader uniform
	ObjectShader->Use();
	ObjectShader->SetUniformMatrix4fv("modelViewProjectionMatrix", mvp);
	ObjectShader->SetUniformFloat("time", 10.0f);

	ObjectShader->UseTexture("wallTexture");

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(TexturedVertex), vertices);									//geef aan dat de posities op deze locatie zitten
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(TexturedVertex), &vertices[0].color);					//geef aan dat de kleuren op deze locatie zitten
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(TexturedVertex), &vertices[0].texCoords);
	glDrawArrays(GL_TRIANGLES, 0, 1);																//en tekenen :)
}

void TexturedObject::GenerateMesh()
{
	if (vertices)
		free(vertices);

	vertices = (TexturedVertex*)malloc(sizeof(TexturedVertex) * 3);
	vertices[0].color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
	vertices[0].texCoords = glm::vec2(0.0f, 0.0f);

	vertices[1].color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	vertices[1].position = glm::vec3(0.5f, 1.0f, 0.0f);
	vertices[1].texCoords = glm::vec2(0.5f, 1.0f);


	vertices[2].color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].position = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[2].texCoords = glm::vec2(1.0f, 0.0f);
}