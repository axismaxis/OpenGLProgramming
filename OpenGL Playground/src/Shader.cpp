#include<Shader.h>
#include <fstream>
#include <string>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#pragma comment(lib, "glew32.lib")

Shader::Shader(const char* vsFileName, const char* fsFileName)
{
	std::ifstream vertexShaderFile(vsFileName);
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	const char* vsRawFileData = vertexShaderData.c_str();

	std::ifstream fragShaderFile(fsFileName);
	std::string fragShaderData((std::istreambuf_iterator<char>(fragShaderFile)), std::istreambuf_iterator<char>());
	const char* fsRawFileData = fragShaderData.c_str();

	GLProgramHandle = glCreateProgram();							// maak een shaderprogramma aan

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);		// maak vertex shader aan
	glShaderSource(vertexId, 1, &vsRawFileData, NULL);		// laat opengl de shader uit de variabele 'vertexShader' halen
	glCompileShader(vertexId);								// compileer de shader
	CheckShaderErrors(vertexId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(GLProgramHandle, vertexId);					// hang de shader aan het shaderprogramma

	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);	// maak fragment shader aan
	glShaderSource(fragmentId, 1, &fsRawFileData, NULL);	// laat opengl de shader uit de variabele 'fragmentShader' halen
	glCompileShader(fragmentId);							// compileer de shader
	CheckShaderErrors(fragmentId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(GLProgramHandle, fragmentId);

	glBindAttribLocation(GLProgramHandle, 0, "a_position");
	glBindAttribLocation(GLProgramHandle, 1, "a_color");
	glBindAttribLocation(GLProgramHandle, 2, "a_texcoord");

	glLinkProgram(GLProgramHandle);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

void Shader::Use()
{
	glUseProgram(GLProgramHandle);
}

void Shader::CreateUniform(const char* uniformName)
{
	int enabledShaders = 0;
	for (auto& keyPair : UniformMap)
	{
		enabledShaders++;
		std::string keyString(keyPair.first);
		std::string uniformNameString(uniformName);
		if (keyString.compare(uniformNameString) == 0)
		{
			//If key value exists, return
			return;
		}
	}

	UniformMap[uniformName] = glGetUniformLocation(GLProgramHandle, uniformName);
}

void Shader::SetUniformMatrix4fv(const char* uniformName, const glm::mat4 uniformValue)
{
	GLuint uniformHandle = UniformMap[uniformName];
	glUniformMatrix4fv(uniformHandle, 1, 0, glm::value_ptr(uniformValue));
}

void Shader::SetUniformMatrix3fv(const char* uniformName, const glm::mat3 uniformValue)
{
	GLuint uniformHandle = UniformMap[uniformName];
	glUniformMatrix3fv(uniformHandle, 1, 0, glm::value_ptr(uniformValue));
}

void Shader::SetUniformVector3fv(const char* uniformName, const glm::vec3 uniformValue)
{
	GLuint uniformHandle = UniformMap[uniformName];
	glUniform3fv(uniformHandle, 1, glm::value_ptr(uniformValue));
}

void Shader::SetUniformFloat(const char* uniformName, const float uniformValue)
{
	GLuint uniformHandle = UniformMap[uniformName];
	glUniform1f(uniformHandle, uniformValue);
}

void Shader::SetUniformTexture(const char* uniformName, const int uniformValue)
{
	GLuint uniformHandle = UniformMap[uniformName];
	glUniform1i(uniformHandle, uniformValue);
}

void Shader::CheckShaderErrors(unsigned int shaderId)
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

void GLAPIENTRY onDebugger(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << message << std::endl;
}


void Shader::EnableDebug(bool enable)
{
	if (enable)
	{
		if (glDebugMessageCallback)
		{
			glDebugMessageCallback(&onDebugger, NULL);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glEnable(GL_DEBUG_OUTPUT);
		}
	}
	else
	{
		if (glDebugMessageCallback)
		{
			glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glDisable(GL_DEBUG_OUTPUT);
		}
	}
}

void GLAPIENTRY Shader::OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << message << std::endl;
}