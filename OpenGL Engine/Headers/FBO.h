#pragma once
#include<Shader.h>

class FBO
{
public:
	FBO(int width, int height);
	void Bind();
	void Render();
	void SetShader(Shader *shader);


	int Width;
	int Height;
	unsigned int FBOId;
	unsigned int FBOTextureId;

private:
	Shader *currentShader;
};