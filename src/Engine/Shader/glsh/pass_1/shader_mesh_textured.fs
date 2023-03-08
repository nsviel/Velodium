#version 330 core

uniform sampler2D fs_texture;

in VS_OUT{
  vec4 vertex_color;
  vec2 tex_coord;
} fs_in;

out vec4 fs_color;


void main()
{
  vec4 frag_tex = texture(fs_texture, fs_in.tex_coord);
  fs_color = frag_tex * fs_in.vertex_color;
}
