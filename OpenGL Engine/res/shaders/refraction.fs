#version 330

in vec3 normal;
in vec3 position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(position - cameraPos);
	vec3 Refraction = refract(I, normalize(normal), ratio);
	gl_FragColor = vec4(texture(skybox, Refraction).rgb, 1.0);
}