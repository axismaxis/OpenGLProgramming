#pragma once
#include<GL/glew.h>
#include<glm.hpp>
#include<map>


class Shader
{
public:
	Shader(const char* vsFileName, const char* fsFileName);
	void Use();
	void EnableDebug(bool enable);
	void CreateUniform(const char* uniformName);
	void SetUniformMatrix4fv(const char* uniformName, const glm::mat4 uniformValue);
	void SetUniformMatrix3fv(const char* uniformName, const glm::mat3 uniformValue);
	void SetUniformFloat(const char* uniformName, const float uniformValue);
	void SetUniformTexture(const char* uniformName, const int uniformValue);

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