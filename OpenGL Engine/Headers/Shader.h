#pragma once
#include<GL/glew.h>
#include<glm.hpp>
#include<map>


class Shader
{
public:
	Shader(const char* vsFileName, const char* fsFileName);
	void Link();
	void Use();
	void EnableDebug(bool enable);
	void CreateUniform(const char* uniformName);
	void SetUniformMatrix4fv(const char* uniformName, const glm::mat4 uniformValue);
	void SetUniformFloat(const char* uniformName, const float uniformValue);

	void CreateTexture(const char* fileName, const char* textureName);
	void UseTexture(const char* textureName);
private:
	void CheckShaderErrors(unsigned int shaderId);
	void GLAPIENTRY OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	GLuint GLProgramHandle;

	std::map<const char*, GLuint> UniformMap;

	/*
	TEMP
	*/
	GLuint textureHandle;

	GLuint modelViewUniformHandle;
	GLuint timeUniformHandle;
};