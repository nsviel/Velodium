#version 330 core

in VS_OUT{
  vec4 vertex_color;
} fs_in;

out vec4 fs_color;

uniform vec3 light_color;


void main()
{
  fs_color = vec4(1.0);
}
