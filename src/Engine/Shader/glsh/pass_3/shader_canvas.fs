#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color;


void main()
{
  out_color = texture(tex_color, vs_tex_coord);
}
