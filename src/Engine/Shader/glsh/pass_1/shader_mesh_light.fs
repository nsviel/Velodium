#version 330 core

in VS_OUT{
  vec4 color;
} fs_in;

out vec4 fs_color;

uniform vec3 light_color;


void main()
{
  //fs_color = vec4(light_color * fs_in.color, 1.0);
  //fs_color = fs_in.color;
  fs_color = vec4(light_color, 1.0);
}
