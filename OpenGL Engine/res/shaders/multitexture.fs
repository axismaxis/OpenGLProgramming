#version 330

uniform sampler2D s_texture;
uniform sampler2D normalMap;
uniform sampler2D secondTexture;
in vec2 texCoord;

void main()
{
    //Apply texture
	gl_FragColor = (texture2D(s_texture, texCoord) + texture2D(secondTexture, texCoord)) / 2;
}