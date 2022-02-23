#version 330 core

in vec2 tex_coordinate;
out vec4 color_fragment;

uniform sampler2D tex_screen;

void main()
{
  color_fragment = texture(tex_screen, tex_coordinate);
}
