#version 330 core
#define PI 3.1415926538

layout (location = 0) out vec4 out_color;

in vec2 vs_tex_coord;

uniform sampler2D tex_depth;
uniform sampler2D tex_color;
uniform sampler2D tex_position;
uniform sampler2D tex_coord_0;
uniform sampler2D tex_coord_1;
uniform sampler2D tex_coord_2;
uniform sampler2D tex_coord_3;
uniform sampler2D tex_coord_4;

uniform int GL_WIDTH;
uniform int GL_HEIGHT;
uniform vec3 CAM_POSE;
uniform mat4 PROJ;
uniform mat4 VIEW;

const int index_size = 9;
float table_index[index_size] = float[](5.0, 6.0, 7.0, 4.0, -1.0, 0.0, 3.0, 2.0, 1.0);



float compute_norm(vec3 pos){
  //---------------------------

  float norm = sqrt(pow(pos.x, 2) + pow(pos.y, 2) + pow(pos.z, 2));

  //---------------------------
  return norm;
}
vec3 unproject(vec2 coord_tex){
  //---------------------------

  //Coord in Raster space to NDC coordinate
  vec2 coord_ndc;
  coord_ndc.x = (2 * coord_tex.x) - 1;
  coord_ndc.y = 1 - (2 * coord_tex.y);

  //Clip to view  space
  float ratio = GL_WIDTH / GL_HEIGHT;
  vec4 coord_view;
  coord_view.x = coord_ndc.x * tan(65 / 2) * ratio;
  coord_view.y = coord_ndc.y * tan(65 / 2);
  coord_view.z = -1;
  coord_view.w = 1;

  //View space to world space
  mat4 view_inv = inverse(VIEW);
  vec4 coord_world = view_inv * coord_view;

  vec3 fct_out  = vec3(coord_world);

  //---------------------------
  return fct_out;
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

  //Get neighbor on all pyramid level
  vec4[8] nn_lvl_0 = compute_lvl_nn(1, tex_coord_0);
  vec4[8] nn_lvl_1 = compute_lvl_nn(2, tex_coord_1);
  vec4[8] nn_lvl_2 = compute_lvl_nn(4, tex_coord_2);
  vec4[8] nn_lvl_3 = compute_lvl_nn(8, tex_coord_3);
  vec4[8] nn_lvl_4 = compute_lvl_nn(16, tex_coord_4);

  //Concatenate the neighbor arrays
  vec4 sector[32];
  for(int i=0; i<8; i++){
    sector[i] = nn_lvl_0[i];
    sector[i+8] = nn_lvl_1[i];
    sector[i+16] = nn_lvl_2[i];
    sector[i+24] = nn_lvl_3[i];
    sector[i+32] = nn_lvl_4[i];
  }



  //Get point to camera vector
  vec3 pixel_pos = texture(tex_position, vs_tex_coord).xyz;
  vec3 tex_world = unproject(vs_tex_coord);
  vec3 x = tex_world - pixel_pos;
  vec3 pt_to_cam = - x / compute_norm(x);

  //for each sector and for each neighbor
  float nn_occlusion_sum = 0;
  for(int i=0; i<8; i++){
    //init variable by the highest value
    float sector_occlusion_min = 2;

    // Get minimal nn visibility per sector
    for(int j=0; j<sector.length(); j++){
      if((sector[j].w - i) < 0.01){
        vec2 nn_coord = sector[j].xy;
        vec3 nn_pos = texture(tex_position, nn_coord).xyz;

        // OK IL FAUT QUE TEX8WOLRD ICI CORRESPONDE AU TEX QUI VIT LE NN !!!!!!
        vec3 y = tex_world - nn_pos;
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
  /*float depth = texture(tex_depth, vs_tex_coord).r;
  //---------------------------

  bool is_visible = false;
  if(depth < 0.999999){
    is_visible = compute_visibility();
  }

  vec4 color = vec4(1);
  if(is_visible){
    color = texture(tex_color, vs_tex_coord);
    gl_FragDepth = texture(tex_depth, vs_tex_coord).r;
  }


/*


  //UNPROECTION BUG SEARCH
  vec2 coord_frag = gl_FragCoord.xy;

  //Coord in Raster space to NDC coordinate
  vec2 coord_ndc;
  coord_ndc.x = (2 * coord_frag.x / GL_WIDTH) - 1;
  coord_ndc.y = 1 - (2 * (coord_frag.y) / GL_HEIGHT);

  //NDC to view  space
  float ratio = GL_WIDTH / GL_HEIGHT;
  vec4 coord_view;
  coord_view.x = coord_ndc.x * tan((65 / 2) * 2 * PI) * ratio;
  coord_view.y = coord_ndc.y * tan((65 / 2) * 2 * PI);
  coord_view.z = -1;
  coord_view.w = 1;

  //View space to world space
  mat4 view_inv = inverse(VIEW);
  vec4 coord_world =  view_inv * coord_view;

  vec3 fct_out  = vec3(coord_world);



  color = vec4(fct_out, 1);
*/
//Get point to camera vector
/*vec3 pixel_pos = texture(tex_coord_0, vs_tex_coord).xyz;
vec3 truc = unproject(vs_tex_coord);
vec3 x = truc - pixel_pos;

float dist = compute_norm(x);

if(depth < 0.999999){
//color = texture(tex_coord_0, vs_tex_coord).xyz;
}*/

  vec4 color = texture(tex_coord_1, vs_tex_coord);

  //---------------------------
  out_color = color;
  //out_color = texture(tex_coord_0, vs_tex_coord);
}
