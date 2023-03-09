#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

out VS_OUT{
  vec4 color;
} vs_out;

uniform mat4 MVP;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  gl_Position = MVP * XYZ;

  //Color
  vs_out.color = in_color;
}
