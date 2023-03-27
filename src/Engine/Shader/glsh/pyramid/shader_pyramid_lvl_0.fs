#version 330 core

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_position;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;
uniform float Z_NEAR;
uniform float Z_FAR;


float compute_depth_normalized(float depth){
  //---------------------------

  float depth_norm = (2.0 * Z_NEAR) / (Z_FAR + Z_NEAR - depth * (Z_FAR - Z_NEAR));

  //---------------------------
  return depth_norm;
}

void main(){
  //---------------------------

  //Depth
  float depth = texture(tex_depth, vs_tex_coord).r;
  float depth_norm = compute_depth_normalized(depth);
  out_color = vec4(depth_norm, 0, 0, 1);

  //Position
  out_position = texture(tex_position, vs_tex_coord);

  //---------------------------
}
