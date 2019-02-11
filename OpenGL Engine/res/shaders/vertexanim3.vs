#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texcoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 modelViewProjectionMatrix;
uniform float time;
out vec3 color;


void main()
{
	color = a_color;
	gl_Position = modelViewProjectionMatrix * vec4(a_position + 
		0.75 * sin(time) * a_normal,
	1);
}