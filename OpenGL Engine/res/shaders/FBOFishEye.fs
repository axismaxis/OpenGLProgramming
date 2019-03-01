#version 330
uniform sampler2D screenTexture;
uniform float time;

in vec2 TexCoords;

const float PI = 3.1415926535;

void main()
{
  float aperture = 200.0;
  float apertureHalf = 0.5 * aperture * (PI / 180.0);
  float maxFactor = 0.8* sin(apertureHalf);
  
  vec2 uv;
  vec2 xy = 2.0 * TexCoords.xy - 1.0;
  float d = length(xy);
  if (d < (2.0-maxFactor))
  {
    d = length(xy * maxFactor);
    float z = sqrt(1.0 - d * d);
    float r = atan(d, z) / PI;
    float phi = atan(xy.y, xy.x);
    
    uv.x = r * cos(phi) + 0.5;
    uv.y = r * sin(phi) + 0.5;
  }
  else
  {
    uv = xy;
  }
  vec4 c = texture2D(screenTexture, uv);
  gl_FragColor = c;
}