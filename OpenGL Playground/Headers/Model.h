#pragma once
#include <gl/GL.h>

struct VertexPNTBT 
{
	float x, y, z;
	float nx, ny, nz;
	float tx, ty;
	float bt1, bt2, bt3, bt4;
};

struct VertexP
{
	float x, y, z;
};

class Model
{
public:
	virtual void Load() = 0;
	virtual void Draw() = 0;

protected:
	GLuint VAOHandle;
};

class Cube : public Model
{
	virtual void Load();
	virtual void Draw();
};