#version 330
uniform sampler2D screenTexture;
uniform float time;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
	vec4 color = texture2D(screenTexture, TexCoords + vec2(0.1 * sin(time + TexCoords.y*2),0.01 * cos(time + TexCoords.x*3)));
	
	FragColor = color;
}