#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coord;

out VS_OUT{
  vec4 color;
  vec2 tex_coord;
} vs_out;

uniform mat4 MVP;
uniform int point_size;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  gl_Position = MVP * XYZ;
  //gl_PointSize = point_size;

  //Color
  vs_out.color = in_color;

  //Texture
  vs_out.tex_coord = in_tex_coord;
}
