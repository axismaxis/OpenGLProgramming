#version 330

uniform sampler2D s_texture;
in vec2 texCoord;

vec4 col(int r, int g, int b)
{
	return vec4(
		1.0 / 255.0 * r,
		1.0 / 255.0 * g,
		1.0 / 255.0 * b,
		1
	);
}

void main()
{
	int coordsX = int(round(texCoord.x * 100));
	int coordsY = int(round(texCoord.y * 200));
	float color;
	if(coordsX % 10 > 8 || coordsY % 10 > 8)
	{
		gl_FragColor = col(127, 113, 94);
	}
	else
	{
		gl_FragColor = col(124, 80, 17);
	}
}