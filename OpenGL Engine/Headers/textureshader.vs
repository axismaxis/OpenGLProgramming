#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 modelViewProjectionMatrix;
uniform float time = 0;

out vec4 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1) * modelViewProjectionMatrix;
	ourColor = aColor;
	TexCoord = aTexCoord;
}