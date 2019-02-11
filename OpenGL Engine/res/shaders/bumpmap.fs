#version 330

uniform sampler2D s_texture;
uniform sampler2D normalMap;
in vec2 texCoord;

void main()
{
	vec3 normal = texture(normalMap, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 lightDirection = normalize(vec3(1,1,1));
	vec3 viewDirection = vec3(0,0,1);
	float shininess = 100.0;

	float ambient = 0.2;
	float diffuse = 0.8 * dot(normal, lightDirection);

	vec3 r = reflect(-lightDirection, normal);

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float factor = ambient + diffuse + specular;

    //Apply texture
	gl_FragColor = texture2D(s_texture, texCoord) *
                     vec4(factor,factor,factor,1.0);
}