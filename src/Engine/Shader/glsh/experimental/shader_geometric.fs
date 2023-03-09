#version 330 core

layout (location = 0) out vec3 out_position;

out VS_OUT{
  vec3 position;
  vec4 color;
  vec2 texture;
} fs_in;


void main()
{
    // store the fragment position vector in the first gbuffer texture
    out_position = fs_in.position;
}
