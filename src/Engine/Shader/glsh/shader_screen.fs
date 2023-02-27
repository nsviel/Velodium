#version 330 core

uniform sampler2D fs_texture;
uniform vec3 lightColor;

in VS_OUT{
  vec4 color;
  vec2 tex_coord;
} fs_in;

out vec4 fs_color;


void main()
{
  vec4 frag_tex = texture(fs_texture, fs_in.tex_coord);
  fs_color = vec4(lightColor, 1) * frag_tex * fs_in.color;
}
