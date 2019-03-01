#pragma once
#include<glm.hpp>

class Vertex
{
public:
	glm::vec3 position;
	glm::vec4 color;
	Vertex(const glm::vec3 &position, const glm::vec4 &color) : position(position), color(color) {}
};

class TexturedVertex
{
public:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	TexturedVertex(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &texCoords) : position(position), color(color), texCoords(texCoords) {}
};