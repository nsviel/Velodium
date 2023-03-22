#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

uniform float Z_NEAR;
uniform float Z_FAR;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform int NN_SIZE;


//FUNCTION 1 - Compute normalized depth
float compute_depth_normalized(float depth){
  //---------------------------

  float depth_norm = (2.0 * Z_NEAR) / (Z_FAR + Z_NEAR - depth * (Z_FAR - Z_NEAR));

  //---------------------------
  return depth_norm;
}

void main()
{
  vec4 pixel_depth = texture(tex_depth, vs_tex_coord);
  //---------------------------

  //Search for neighbor
  bool is_nearest = true;
  vec2 pixel_size = 1 / vec2(GL_WIDTH, GL_HEIGHT);

  for(int i=-1*NN_SIZE; i<2*NN_SIZE; i++){
    for(int j=-1*NN_SIZE; j<2*NN_SIZE; j++){
      vec2 nn_coord = vs_tex_coord + vec2(pixel_size.x * i, pixel_size.y * j);
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
    pixel_color = vec4(1);
  }

  //---------------------------
  out_color = pixel_color;
}
