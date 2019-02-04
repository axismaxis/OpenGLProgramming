#version 330

uniform float time = 0;

in vec3 color;
out vec3 fragColor;

void main()
{
	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	fragColor = vec3(gray);
}