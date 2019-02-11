#include<GameObject.h>
#include<GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <time.h>

GameObject::GameObject(const char* objModelFileName, glm::vec3 position)
{
	Position = position;
	rotationSpeed = std::rand() % 100 / 100.0f;
	mesh = new ObjModel(objModelFileName);
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
	if(rotating) rotation += fElapsedTime / 500.0f * rotationSpeed;
}

void GameObject::Draw(glm::mat4 view, glm::mat4 projection, float fTotalTime)
{
	//Only do these in the object
	glm::mat4 model = glm::translate(glm::mat4(1), Position);
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

	// Set MVP in shader uniform
	ObjectShader->Use();
	ObjectShader->SetUniformMatrix4fv("modelMatrix", model);
	ObjectShader->SetUniformMatrix4fv("viewMatrix", view);
	ObjectShader->SetUniformMatrix4fv("projectionMatrix", projection);
	ObjectShader->SetUniformMatrix3fv("normalMatrix", normalMatrix);
	ObjectShader->SetUniformFloat("time", fTotalTime);
	ObjectShader->SetUniformTexture("s_texture", 0);
	ObjectShader->SetUniformTexture("normalMap", 1);

	mesh->draw();
}

void GameObject::ToggleRotate()
{
	rotating = !rotating;
}
