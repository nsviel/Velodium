#version 330 core
#define PI 3.1415926538

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_posit_0;
uniform sampler2D tex_posit_1;
uniform sampler2D tex_posit_2;
uniform sampler2D tex_posit_3;
uniform sampler2D tex_posit_4;

uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform vec3 CAM_POSE;


float compute_sector_index(float l, float k){
  // l et k doivent etre int entre [-1, 1]
  //---------------------------

  float alpha = atan(l, k);

  if(alpha < 0){
    alpha = alpha + 2 * PI;
  }

  float theta = (8 * alpha) / (2 * PI);
  float index = round(theta);

  //---------------------------
  return index;
}
float compute_norm(vec3 pos){
  //---------------------------

  float norm = sqrt(pow(pos.x, 2) + pow(pos.y, 2) + pow(pos.z, 2));

  //---------------------------
  return norm;
}
bool compute_visibility(){
  vec4 sector[100];
  //---------------------------

  //Compute neighbor sector index
  int cpt = 0;
  vec2 pixel_size = vec2(1) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(i != 0 && j!= 0){
        float index = compute_sector_index(i, j);

        float l = pixel_size.x * i;
        float k = pixel_size.y *j;

        vec2 NN_coord = vs_tex_coord + vec2(l, k);
        vec4 NN_position = texture(tex_posit_0, NN_coord);

        sector[cpt] = vec4(NN_position.xyz, index);
        cpt = cpt + 1;
      }
    }
  }

  pixel_size = vec2(2) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(i != 0 && j!= 0){
        float index = compute_sector_index(i, j);

        float l = pixel_size.x * i;
        float k = pixel_size.y *j;

        vec2 NN_coord = vs_tex_coord + vec2(l, k);
        vec4 NN_position = texture(tex_posit_1, NN_coord);

        sector[cpt] = vec4(NN_position.xyz, index);
        cpt = cpt + 1;
      }
    }
  }

  pixel_size = vec2(4) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(i != 0 && j!= 0){
        float index = compute_sector_index(i, j);

        float l = pixel_size.x * i;
        float k = pixel_size.y *j;

        vec2 NN_coord = vs_tex_coord + vec2(l, k);
        vec4 NN_position = texture(tex_posit_2, NN_coord);

        sector[cpt] = vec4(NN_position.xyz, index);
        cpt = cpt + 1;
      }
    }
  }

  pixel_size = vec2(8) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(i != 0 && j!= 0){
        float index = compute_sector_index(i, j);

        float l = pixel_size.x * i;
        float k = pixel_size.y *j;

        vec2 NN_coord = vs_tex_coord + vec2(l, k);
        vec4 NN_position = texture(tex_posit_3, NN_coord);

        sector[cpt] = vec4(NN_position.xyz, index);
        cpt = cpt + 1;
      }
    }
  }

  pixel_size = vec2(16) / vec2(GL_WIDTH, GL_HEIGHT);
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(i != 0 && j!= 0){
        float index = compute_sector_index(i, j);

        float l = pixel_size.x * i;
        float k = pixel_size.y *j;

        vec2 NN_coord = vs_tex_coord + vec2(l, k);
        vec4 NN_position = texture(tex_posit_4, NN_coord);

        sector[cpt] = vec4(NN_position.xyz, index);
        cpt = cpt + 1;
      }
    }
  }

  //Get point to camera vector
  vec3 pixel_pos = texture(tex_posit_0, vs_tex_coord).xyz;
  vec3 pt_to_cam = - (CAM_POSE - pixel_pos) / compute_norm(CAM_POSE - pixel_pos);

  //for each sector and for each neighbor
  float visibility_total_sum = 0;
  for(int i=0; i<8; i++){
    //init variable by the highest value
    float visibility_sector_min = 2;

    // Get minimal nn visibility per sector
    for(int j=0; j<cpt; j++){
      if((sector[j].w - i) < 0.01){
        vec3 nn_pos = sector[j].xyz;

        vec3 x = CAM_POSE - pixel_pos;
        vec3 y = CAM_POSE - nn_pos;
        vec3 nn_cone = (y - x) / compute_norm(y - x);
        float visibility_nn = 1 - dot(nn_cone, pt_to_cam);

        if(visibility_nn < visibility_sector_min){
          visibility_sector_min = visibility_nn;
        }
      }
    }

    //Get sum of all sector visibility
    visibility_total_sum += visibility_sector_min;
  }
  float visibility = visibility_total_sum / 8;

  bool is_visible;
  if(visibility < 0.5){
    is_visible = false;
  }else{
    is_visible = true;
  }

  //---------------------------
  return is_visible;
}

void main(){
  //---------------------------

  bool is_visible = compute_visibility();
  vec4 color = vec4(1);
  if(is_visible){
    color = vec4(1, 0, 0, 1);
  }
  out_color = color;

  //---------------------------
}
