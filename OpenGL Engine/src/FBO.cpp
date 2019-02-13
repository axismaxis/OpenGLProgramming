#include "FBO.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <vector>


FBO::FBO(int width, int height)
{
	Width = width;
	Height = height;

	glGenTextures(1, &FBOTextureId);
	glBindTexture(GL_TEXTURE_2D, FBOTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	glGenFramebuffers(1, &FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTextureId, 0);

	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
}

void FBO::Render()
{
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(-1, -1));
	verts.push_back(glm::vec2(1, -1));
	verts.push_back(glm::vec2(1, 1));
	verts.push_back(glm::vec2(-1, 1));

	currentShader->Use();
	currentShader->SetUniformFloat("time", glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	currentShader->SetUniformTexture("s_texture", 0);

	glBindTexture(GL_TEXTURE_2D, FBOTextureId);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);									//geef aan dat de posities op deze locatie zitten
	glDrawArrays(GL_QUADS, 0, verts.size());
}

void FBO::SetShader(Shader *shader)
{
	currentShader = shader;
}