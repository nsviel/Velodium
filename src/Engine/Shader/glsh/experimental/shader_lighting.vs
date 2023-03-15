#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 3) in vec3 in_normal;

out VS_OUT{
  vec4 vertex_color;
  vec3 vertex_normal;
  vec3 vertex_position;
} vs_out;

uniform mat4 MVP;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  gl_Position = MVP * XYZ;
  vs_out.vertex_position = in_position;

  //Color
  vs_out.vertex_color = in_color;

  //Normal
  vs_out.vertex_normal = in_normal;
}
