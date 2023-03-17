#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_tex_coord;
layout (location = 3) in vec3 in_normal;

out VS_OUT{
  vec4 color;
  vec3 position;
  vec2 tex_coord;
  vec3 normal;
} vs_out;

uniform mat4 MVP;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  gl_Position = MVP * XYZ;
  vs_out.position = XYZ.xyz;

  //Color
  vs_out.color = in_color;

  //Texture
  vs_out.tex_coord = in_tex_coord;

  //Normal
  vs_out.normal = in_normal;
}
