#version 330
layout (location = 0) in vec3 a_position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 TexCoords;

void main()
{
	TexCoords = a_position;
	gl_Position = projectionMatrix * viewMatrix * vec4(a_position,1);
}