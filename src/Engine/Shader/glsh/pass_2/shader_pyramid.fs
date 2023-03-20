#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

uniform float A;
uniform float B;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;


//FUNCTION 1 - Compute normalized depth
float compute_depth_normalized(float depth){
  //---------------------------
  float A = 1.0002;
  float B = -0.20002;

  // depth: Linear depth, in world units
  // depth_norm: normalized depth between [0, 1]
  float depth_norm = 0.5 * (A * depth + B) / depth + 0.5;

  //---------------------------
  return depth_norm;
}

//FUNCTION 2 - Compute neighbor influence
vec2 neighbor_contribution(float depth_norm, vec2 offset) {
  //---------------------------

  // get normalized depth at texture offseted coordinate
  vec2 NN_coord = vs_tex_coord + offset;
  vec4 depth_NN_rgba = texture(tex_depth, NN_coord);
  float depth_NN_norm = compute_depth_normalized(depth_NN_rgba.r);

  // interpolate the two adjacent depth values
  float NN_contrib = max(0.0, log2(depth_norm) - log2(depth_NN_norm));

  //---------------------------
  return vec2(NN_contrib, 1.0);
}

void main()
{

  vec4 texel = texture(tex_color, vs_tex_coord);
  vec4 color_rgba = vec4(vec3(1.0 - texel.rgb), 1.0);
  out_color = vec4(color_rgba);
}
