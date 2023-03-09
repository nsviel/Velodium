#version 330 core

layout(location = 0) in vec3 in_position;
out VS_OUT{
  vec3 position;
  vec4 color;
  vec2 texture;
} vs_out;

uniform mat4 MVP;


void main()
{
  //Location
  vec4 XYZ = vec4(in_position, 1.0);
  vec4 XYZ_s = MVP * XYZ;
  gl_Position = XYZ_s
  vs_out.position = XYZ_s

  //Color
  vs_out.color = in_color;

  //Texture
  vs_out.texture = in_texture;
}
