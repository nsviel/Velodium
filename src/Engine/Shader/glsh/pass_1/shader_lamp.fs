#version 330 core

layout (location = 0) out vec4 out_color;

in VS_OUT{
  vec4 vertex_color;
} fs_in;

uniform vec3 light_color;


void main()
{
  out_color = vec4(light_color, 1.0);
}
