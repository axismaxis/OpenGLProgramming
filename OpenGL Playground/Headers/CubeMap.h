#pragma once



class CubeMap
{
public:
	CubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front);
	
	unsigned int vaoID;
	unsigned int textureID;

private:
	void GenerateVAO();
};