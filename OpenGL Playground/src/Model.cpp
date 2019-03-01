#include <gl/glew.h>
#include "Model.h"

void Cube::Load()
{
	VertexP vertices[] = {
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0
	};

	glGenVertexArrays(1, &VAOHandle);
	glBindVertexArray(VAOHandle);

	GLuint VBOHandle;
	glGenBuffers(1, &VBOHandle);
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexP), 0);

	glBindVertexArray(0);
}

void Cube::Draw()
{
	glBindVertexArray(VAOHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}