#version 330 core

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_data_py;

uniform float Z_NEAR;
uniform float Z_FAR;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform int NN_SIZE;


float compute_depth_normalized(float depth){
  //---------------------------

  float depth_norm = (2.0 * Z_NEAR) / (Z_FAR + Z_NEAR - depth * (Z_FAR - Z_NEAR));

  //---------------------------
  return depth_norm;
}

void main(){
  vec4 pixel_data = texture(tex_data_py, vs_tex_coord);
  vec3 pixel_position = pixel_data.xyz;
  float pixel_depth = pixel_data.w;
  //---------------------------

  //GEt pixel size
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);

  //Retrieve cube position
  vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  vec2 cube_pos = (floor(tex_coord_dim / NN_SIZE) * NN_SIZE) / vec2(GL_WIDTH, GL_HEIGHT);

  //Check if current pixel is the nearest
  vec4 nearest_data = pixel_data;
  for(int i=0; i<NN_SIZE; i++){
    for(int j=0; j<NN_SIZE; j++){
      //Neigbor data
      vec2 NN_coord = cube_pos + vec2(pixel_size.x * i, pixel_size.y * j);
      vec4 NN_data = texture(tex_data_py, NN_coord);
      float NN_depth = NN_data.w;

      //Normalized depth data
      float NN_depth_norm = compute_depth_normalized(NN_depth);
      float pixel_depth_norm = compute_depth_normalized(nearest_data.w);

      if(NN_depth_norm < 0.95 && NN_depth_norm < pixel_depth_norm){
        nearest_data = NN_data;
      }
    }
  }

  //---------------------------
  out_color = nearest_data;
}
