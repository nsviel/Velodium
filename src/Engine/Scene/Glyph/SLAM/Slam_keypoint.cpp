#include "Slam_keypoint.h"


//Constructor / destructor
Slam_keypoint::Slam_keypoint(){
  //---------------------------

  this->color = vec4(0.7f, 0.1f, 0.1f, 1.0f);
  this->visibility = false;

  //---------------------------
}
Slam_keypoint::~Slam_keypoint(){}

void Slam_keypoint::create_keypoint(Subset* subset){
  Glyph keypoint;
  //---------------------------

  //Create glyph
  keypoint.name = "keypoint";
  keypoint.draw_size = 5;
  keypoint.draw_type = "point";
  keypoint.color_unique = color;
  keypoint.visibility = visibility;

  //---------------------------
  subset->keypoint = keypoint;
}
void Slam_keypoint::update_keypoint_location(Subset* subset){
  Glyph* keypoint = &subset->keypoint;
  //---------------------------

  vector<vec3>& XYZ_key = keypoint->location;
  vector<vec4>& RGB_key = keypoint->color;
  vector<float>& ts_key = keypoint->timestamp;

  //Construct glyph
  RGB_key.clear();
  for(int i=0; i<ts_key.size(); i++){
    vec4 rgb = vec4(ts_key[i], 1 - ts_key[i], color[2], 1);
    RGB_key.push_back(rgb);
  }

  //---------------------------
}
void Slam_keypoint::update_keypoint_normal(Subset* subset){
  Glyph* normal = &subset->normal;
  Glyph* keypoint = &subset->keypoint;
  //---------------------------

  //Get vector values
  vector<vec3>& xyz_k = keypoint->location;
  vector<vec3>& Nxyz_k = keypoint->normal;
  vector<vec3>& xyz_n = normal->location;
  vector<vec4>& rgb_n = normal->color;

  //Check vector length
  if(xyz_k.size() == 0 || Nxyz_k.size() == 0 || Nxyz_k.size() != xyz_k.size()){
    return;
  }

  //Clear old normal values
  xyz_n.clear();
  rgb_n.clear();

  //Construct normal
  float lgt = 0.05 * normal->draw_size;
  for(int i=0; i<xyz_k.size(); i++){
    vec3& xyz = xyz_k[i];
    vec3& nxyz = Nxyz_k[i];

    vec3 n_vec = vec3(xyz.x + nxyz.x * lgt, xyz.y + nxyz.y * lgt, xyz.z + nxyz.z * lgt);

    xyz_n.push_back(xyz);
    xyz_n.push_back(n_vec);

    rgb_n.push_back(normal->color_unique);
    rgb_n.push_back(normal->color_unique);
  }

  //---------------------------
}
