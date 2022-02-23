#version 330 core

layout (location = 0) in vec2 in_position;
layout (location = 2) in vec2 in_tex_coord;

out vec2 frag_tex_coord;

void main()
{
  gl_Position = vec4(in_position.x, in_position.y, 0.0, 1.0);
  frag_tex_coord = in_tex_coord;
}
