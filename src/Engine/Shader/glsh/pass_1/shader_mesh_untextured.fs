#version 330 core

in VS_OUT{
  vec4 vertex_color;
} fs_in;

out vec4 fs_color;


void main()
{
  fs_color = fs_in.vertex_color;
}
