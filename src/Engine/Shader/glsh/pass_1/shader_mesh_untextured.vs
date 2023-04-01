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
  vec4 XYZ_p = MVP * XYZ;
  gl_Position = XYZ_p;
  vs_out.position = XYZ_p.xyz;

  //Color
  vs_out.color = in_color;

  //Normal
  vs_out.normal = in_normal;
}
