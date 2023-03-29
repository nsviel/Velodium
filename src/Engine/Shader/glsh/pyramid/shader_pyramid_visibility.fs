#version 330 core
#define PI 3.1415926538

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_color;
uniform sampler2D tex_posit_0;
uniform sampler2D tex_posit_1;
uniform sampler2D tex_posit_2;
uniform sampler2D tex_posit_3;
uniform sampler2D tex_posit_4;

uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform vec3 CAM_POSE;
uniform mat4 PROJ;

const int index_size = 9;
float table_index[index_size] = float[](5.0, 6.0, 7.0, 4.0, -1.0, 0.0, 3.0, 2.0, 1.0);



float compute_norm(vec3 pos){
  //---------------------------

  float norm = sqrt(pow(pos.x, 2) + pow(pos.y, 2) + pow(pos.z, 2));

  //---------------------------
  return norm;
}
vec4[8] compute_lvl_nn(int nn_size, sampler2D tex_pos){
  vec4 nn_level[8];
  //---------------------------

  int cpt = 0;
  vec2 pixel_size = vec2(nn_size) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      float index = table_index[cpt];
      vec2 NN_coord = vs_tex_coord + vec2(pixel_size.x * i, pixel_size.y * j);
      vec4 NN_position = texture(tex_pos, NN_coord);

      nn_level[cpt] = vec4(NN_position.xyz, index);
      cpt ++;
    }
  }

  //---------------------------
  return nn_level;
}
bool compute_visibility(){
  //---------------------------

  vec4[8] nn_lvl_0 = compute_lvl_nn(1, tex_posit_0);
  vec4[8] nn_lvl_1 = compute_lvl_nn(2, tex_posit_1);
  vec4[8] nn_lvl_2 = compute_lvl_nn(4, tex_posit_2);
  vec4[8] nn_lvl_3 = compute_lvl_nn(8, tex_posit_3);
  vec4[8] nn_lvl_4 = compute_lvl_nn(16, tex_posit_4);

  //Concatenate the nn arrays
  vec4 sector[32];
  for(int i=0; i<8; i++){
    sector[i] = nn_lvl_0[i];
    sector[i+8] = nn_lvl_1[i];
    sector[i+16] = nn_lvl_2[i];
    sector[i+24] = nn_lvl_3[i];
    sector[i+32] = nn_lvl_4[i];
  }



  //Get point to camera vector
  vec3 pixel_pos = texture(tex_posit_0, vs_tex_coord).xyz;
  vec3 x = CAM_POSE - pixel_pos;
  vec3 pt_to_cam = - x / compute_norm(x);

  //for each sector and for each neighbor
  float nn_occlusion_sum = 0;
  for(int i=0; i<8; i++){
    //init variable by the highest value
    float sector_occlusion_min = 2;

    // Get minimal nn visibility per sector
    for(int j=0; j<sector.length(); j++){
      if((sector[j].w - i) < 0.01){
        vec3 nn_pos = sector[j].xyz;

        vec3 y = CAM_POSE - nn_pos;
        vec3 nn_cone = (y - x) / compute_norm(y - x);
        float nn_occlusion = 1 - dot(nn_cone, pt_to_cam);

        if(nn_occlusion < sector_occlusion_min){
          sector_occlusion_min = nn_occlusion;
        }
      }
    }

    //Get sum of all sector visibility
    nn_occlusion_sum += sector_occlusion_min;
  }
  float pixel_occ = nn_occlusion_sum / 8;

  bool is_visible;
  if(pixel_occ < 0.1){
    is_visible = false;
  }else{
    is_visible = true;
  }

  //---------------------------
  return is_visible;
}

void main(){
  float depth = texture(tex_depth, vs_tex_coord).r;
  //---------------------------

  bool is_visible = false;
  if(depth < 0.98){
    is_visible = compute_visibility();
  }

  vec4 color = vec4(1);
  if(is_visible){
    color = texture(tex_color, vs_tex_coord);
    gl_FragDepth = texture(tex_depth, vs_tex_coord).r;
  }

  //---------------------------
  out_color = color;
}
