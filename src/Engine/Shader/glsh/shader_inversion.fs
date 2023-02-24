#version 330 core

in vec2 vs_tex_coord;
out vec4 fs_color;

uniform sampler2D tex_color;


void main()
{
  vec4 color_rgba = vec4(vec3(1.0 - texture(tex_color, vs_tex_coord)), 1.0);
  fs_color = vec4(color_rgba);
}
