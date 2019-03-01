#version 330
uniform sampler2D screenTexture;
uniform float time;

in vec2 TexCoords;

out vec4 FragColor;

const float radius = 2;
const float angle = 0.8;
const vec2 center = vec2(0.5, 0.5);

void main()
{
	vec2 tc = 2.0 * TexCoords.xy - 1.0;;
	float dist = length(tc);
	tc -= center;
	if (dist < radius) 
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	tc += center;
	vec3 color = texture2D(screenTexture, tc).rgb;

	FragColor = vec4(color, 1.0);
}