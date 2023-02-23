#version 330 core

in vec4 vs_color;
in vec2 vs_texture;

out vec4 fs_color;

uniform sampler2D fs_texture;


void main()
{
  //fs_color = vec4(vs_color);
  fs_color = texture(fs_texture, vs_texture) * vs_color;
}
