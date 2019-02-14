#include "FBO.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

FBO::FBO(int width, int height)
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	Width = width;
	Height = height;
	
	//Generate new frame buffer and bind it
	glGenFramebuffers(1, &FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOId);

	//Generate texture to render to
	glGenTextures(1, &FBOTextureId);
	glBindTexture(GL_TEXTURE_2D, FBOTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTextureId, 0);

	//Generate render buffer object 
	glGenRenderbuffers(1, &RBOId);
	glBindRenderbuffer(GL_RENDERBUFFER, RBOId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOId); 
	
	//Check if FBO is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is incomplete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
}

void FBO::Render()
{
	currentShader->Use();
	currentShader->SetUniformFloat("time", glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	currentShader->SetUniformTexture("screenTexture", 0);

	glBindVertexArray(QuadVAO);
	glBindTexture(GL_TEXTURE_2D, FBOTextureId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FBO::SetShader(Shader *shader)
{
	currentShader = shader;
}