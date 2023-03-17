#version 330 core

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_position;
layout (location = 2) out vec4 out_normal;

in VS_OUT{
  vec4 color;
  vec3 position;
  vec2 tex_coord;
  vec3 normal;
} fs_in;


void main()
{
  //Color
  out_color = fs_in.color;

  //Position
  out_position = vec4(fs_in.position, 1);

  //Normal
  out_normal = vec4(fs_in.normal, 1);
}
