#version 330 core

in vec2 vs_tex_coord;
out vec4 fs_color;

uniform sampler2D tex_color;


void main()
{
  fs_color = texture(tex_color, vs_tex_coord);
}
