#version 330 core

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec2 out_texture;
layout (location = 2) out vec3 out_normal;

in VS_OUT{
  vec3 position;
  vec4 color;
  vec2 tex_coord;
  vec3 normal;
} fs_in;

out vec4 fs_tex_color;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;


void main()
{
  //Position
  //out_position = fs_in.position;

  vec4 color_rgba = vec4(vec3(1.0 - texture(tex_color, fs_in.tex_coord)), 1.0);
  color_rgba = texture(tex_depth, fs_in.tex_coord);
  fs_tex_color = vec4(fs_in.position, 1);
}
