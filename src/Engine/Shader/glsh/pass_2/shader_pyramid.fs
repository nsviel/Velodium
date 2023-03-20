#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

uniform float Z_NEAR;
uniform float Z_FAR;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;


//FUNCTION 1 - Compute normalized depth
float compute_depth_normalized(float depth){
  //---------------------------

  float depth_norm = (2.0 * Z_NEAR) / (Z_FAR + Z_NEAR - depth * (Z_FAR - Z_NEAR));

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
  vec4 pixel_depth = texture(tex_depth, vs_tex_coord);
  //---------------------------

  //Search for neighbor
  bool is_nearest = true;
  vec2 texel_size = 1 / vec2(GL_WIDTH, GL_HEIGHT);

  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      vec2 nn_coord = vs_tex_coord + vec2(texel_size.x * i, texel_size.y * j);
      vec4 NN_depth = texture(tex_depth, nn_coord);
      float NN_depth_norm = compute_depth_normalized(NN_depth.r);

      if(NN_depth_norm < 0.95 && NN_depth.r < pixel_depth.r){
        is_nearest = false;
      }
    }
  }

  //Get normalized pixel depth
  float pixel_depth_norm = compute_depth_normalized(pixel_depth.r);

  //Retrieve fragment pixel color
  vec4 pixel_color;
  if(pixel_depth_norm > 0.95){
    pixel_color = texture(tex_color, vs_tex_coord);;
  }else if(is_nearest){
    pixel_color = vec4(0, 0, pixel_depth_norm, 1);
  }else if(is_nearest == false){
    pixel_color = vec4(pixel_depth_norm, 0, 0, 1);
  }

  //---------------------------
  out_color = pixel_color;
}
