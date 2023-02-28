#include "Keypoint.h"


//Constructor / destructor
Keypoint::Keypoint(){
  //---------------------------

  this->color = vec4(0.7f, 0.1f, 0.1f, 1.0f);
  this->visibility = false;

  //---------------------------
}
Keypoint::~Keypoint(){}

void Keypoint::create_keypoint(Subset* subset){
  Glyph keypoint;
  //---------------------------

  //Create glyph
  keypoint.name = "keypoint";
  keypoint.draw_size = 10;
  keypoint.draw_type = "point";
  keypoint.color_unique = color;
  keypoint.visibility = visibility;

  //---------------------------
  subset->glyphs.insert({"keypoint", keypoint});
}
void Keypoint::update_keypoint_location(Subset* subset){
  Glyph* keypoint = &subset->glyphs["keypoint"];
  //---------------------------

  vector<vec3>& XYZ_key = keypoint->xyz;
  vector<vec4>& RGB_key = keypoint->rgb;
  vector<float>& ts_key = keypoint->ts;

  //Construct glyph
  RGB_key.clear();
  for(int i=0; i<ts_key.size(); i++){
    vec4 rgb = vec4(ts_key[i], 1 - ts_key[i], color[2], 1);
    RGB_key.push_back(rgb);
  }

  //---------------------------
}
void Keypoint::update_keypoint_normal(Subset* subset){
  Glyph* normal = &subset->glyphs["normal"];
  Glyph* keypoint = &subset->glyphs["keypoint"];
  //---------------------------

  //Get vector values
  vector<vec3>& xyz_k = keypoint->xyz;
  vector<vec3>& Nxyz_k = keypoint->Nxyz;
  vector<vec3>& xyz_n = normal->xyz;
  vector<vec4>& rgb_n = normal->rgb;

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
