#version 330 core

layout (location = 0) out vec4 out_color;

uniform sampler2D fs_texture;

in VS_OUT{
  vec4 vertex_color;
  vec2 tex_coord;
} fs_in;


void main()
{
  vec4 frag_tex = texture(fs_texture, fs_in.tex_coord);
  out_color = frag_tex * fs_in.vertex_color;
}
