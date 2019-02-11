#version 330

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	//gl_FragColor = vec4(1,1,1,1);
	gl_FragColor = texture(skybox, TexCoords);
}