#version 330 core//---------------------------

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_position;

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
bool compute_is_nearest(vec4 pixel_depth){
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);
  //---------------------------

  //Retrieve cube position
  vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  vec2 cube_pos = (floor(tex_coord_dim / NN_SIZE) * NN_SIZE) / vec2(GL_WIDTH, GL_HEIGHT);

  //Check if current pixel is the nearest
  bool is_nearest = true;
  for(int i=0; i<NN_SIZE; i++){
    for(int j=0; j<NN_SIZE; j++){
      vec2 NN_coord = cube_pos + vec2(pixel_size.x * i, pixel_size.y * j);
      vec4 NN_depth = texture(tex_depth, NN_coord);

      //Normalize depths
      float NN_depth_norm = compute_depth_normalized(NN_depth.r);
      float pixel_depth_norm = compute_depth_normalized(pixel_depth.r);

      if(NN_depth_norm < 0.95 && NN_depth_norm < pixel_depth_norm){
        is_nearest = false;
      }
    }
  }

  //---------------------------
  return is_nearest;
}
float compute_norm(vec3 pos){
  //---------------------------

  float norm = sqrt(pow(pos.x, 2) + pow(pos.y, 2) + pow(pos.z, 2));

  //---------------------------
  return norm;
}
void compute_pixel_visibility(){
  //---------------------------

  vec3 pixel_pos = texture(tex_position, vs_tex_coord).rgb;
  vec3 pt_to_cam = - pixel_pos / compute_norm(pixel_pos);

  //for each sector and for eachneighbor
  int nb_sector = 8;
  float w_m_sum = 0;
  for(int i=0; i<nb_sector; i++){
    int nb_nn = 1;
    float ww_m_min = 2;

    for(int i=0; i<nb_nn; i++){
      vec3 nn = vec3(0);

      vec3 nn_cone = (nn - pixel_pos) / compute_norm(nn - pixel_pos);
      float w_nn = 1 - dot(nn_cone, pt_to_cam);

      if(w_nn < ww_m_min){
        ww_m_min = w_nn;
      }
    }

    w_m_sum += ww_m_min;
  }
  float visibility = w_m_sum / nb_sector;

  bool is_visible;
  if(visibility > 0.1){
    is_visible = false;
  }else{
    is_visible = true;
  }

  //---------------------------
}

void main(){
  vec4 pixel_depth = texture(tex_depth, vs_tex_coord);
  vec4 pixel_position = texture(tex_position, vs_tex_coord);
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);
  //---------------------------

  //Retrieve cube position
  vec2 tex_coord_dim = vs_tex_coord * vec2(GL_WIDTH, GL_HEIGHT);
  vec2 cube_pos = (floor(tex_coord_dim / NN_SIZE) * NN_SIZE) / vec2(GL_WIDTH, GL_HEIGHT);

  //Check if current pixel is the nearest
  bool is_nearest = true;
  float nearest_depth = pixel_depth.r;
  vec4 nearest_position = pixel_position;
  for(int i=0; i<NN_SIZE; i++){
    for(int j=0; j<NN_SIZE; j++){
      //Neigbor data
      vec2 NN_coord = cube_pos + vec2(pixel_size.x * i, pixel_size.y * j);
      float NN_depth = texture(tex_depth, NN_coord).r;

      //Normalized depth data
      float NN_depth_norm = compute_depth_normalized(NN_depth);
      float pixel_depth_norm = compute_depth_normalized(pixel_depth.r);

      if(NN_depth_norm < 0.95 && NN_depth_norm < pixel_depth_norm){
        vec4 NN_position = texture(tex_position, vs_tex_coord);

        nearest_depth = NN_depth;
        nearest_position = NN_position;
      }
    }
  }

  //---------------------------
  out_color = vec4(nearest_position.rgb, nearest_depth);
}
