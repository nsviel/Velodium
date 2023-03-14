#version 330 core

in VS_OUT{
  vec4 color;
} fs_in;

out vec4 fs_color;

uniform vec3 light_color;


void main()
{
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * light_color;

  vec3 result = ambient * objectColor;
  fs_color = vec4(result, 1.0);
}
