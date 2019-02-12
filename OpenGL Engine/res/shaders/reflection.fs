#version 330

in vec3 normal;
in vec3 position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(position - cameraPos);
	vec3 R = reflect(I, normalize(normal));
	gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
}