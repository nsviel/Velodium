#include "SLAM_utility.h"


void make_translation(vector<vec3>& XYZ, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //Apply
  make_Transformation_atomic(XYZ, vec3(0, 0, 0), translation);

  //---------------------------
}
void make_rotation_origin(vector<vec3>& XYZ, mat4 R){
  //---------------------------

  vec3 COM = vec3(0, 0, 0);
  make_Transformation_atomic(XYZ, COM, R);

  //---------------------------
}
void make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation){
  //---------------------------

  //#pragma omp parallel for
  for(int i=0; i<XYZ.size(); i++){
    vec4 XYZ_hom = vec4(XYZ[i].x - COM.x, XYZ[i].y - COM.y, XYZ[i].z - COM.z, 1.0);
    vec4 XYZ_tr = XYZ_hom * Transformation;

    XYZ[i] = vec3(XYZ_tr.x + COM.x, XYZ_tr.y + COM.y, XYZ_tr.z + COM.z);
  }

  //---------------------------
}
