#version 330 core

layout (location = 0) out vec4 out_color;

in VS_OUT{
  vec4 vertex_color;
} fs_in;


void main()
{
  out_color = fs_in.vertex_color;
}
