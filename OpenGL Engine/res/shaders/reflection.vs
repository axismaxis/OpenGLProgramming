#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 normal;
out vec3 position;

void main()
{
	//normal = normalMatrix * a_normal;
	normal = mat3(transpose(inverse(modelMatrix))) * a_normal;
	position = vec3(modelMatrix * vec4(a_position, 1.0));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
}