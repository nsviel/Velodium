#version 330 core

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_position;
layout (location = 2) out vec4 out_normal;

in VS_OUT{
  vec4 position;
  vec4 color;
  vec2 tex_coord;
  vec3 normal;
} fs_in;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;


void main()
{
  //Color
  vec4 color_rgba = vec4(vec3(1.0 - texture(tex_color, fs_in.tex_coord)), 1.0);
  color_rgba = texture(tex_depth, fs_in.tex_coord);
  out_color = vec4(color_rgba);

  //Position
  out_position = fs_in.position;

  //Normal
  out_normal = vec4(fs_in.normal, 1);
}
