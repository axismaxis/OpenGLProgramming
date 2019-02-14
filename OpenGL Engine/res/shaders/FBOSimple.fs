#version 330
uniform sampler2D screenTexture;
uniform float time;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
	vec3 color = texture(screenTexture, TexCoords).rgb;

	FragColor = vec4(color, 1.0);
}