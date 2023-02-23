#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texture;

out vec4 vs_color;
out vec2 vs_texture;

uniform mat4 MVP;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  gl_Position = MVP * XYZ;

  //Color
  vs_color = in_color;

  //Texture
  vs_texture = in_texture;
}
