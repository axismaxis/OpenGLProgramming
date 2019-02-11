#pragma once



class CubeMap
{
public:
	CubeMap(const char* top, const char* bottom, const char* left, const char* right, const char* front, const char* back);
	
	unsigned int vaoID;
	unsigned int textureID;

private:
	void GenerateVAO();
};