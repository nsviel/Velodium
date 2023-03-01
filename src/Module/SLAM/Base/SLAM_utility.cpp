#include "SLAM_utility.h"

namespace SLAM_utility{

Eigen::Vector3d fct_centroid(std::vector<Eigen::Vector3d>& XYZ){
  Eigen::Vector3d centroid = Eigen::Vector3d::Zero();
  int size = XYZ.size();
  //---------------------------

  for(int i=0; i<size; i++){
    for(int j=0; j<3; j++){
      centroid(j) += XYZ[i](j);
    }
  }

  for(int i=0; i<3; i++){
    centroid(i) /= (double) size;
  }

  //---------------------------
  return centroid;
}
Eigen::Matrix3d fct_covarianceMat(std::vector<Eigen::Vector3d>& vec){
  //---------------------------

  // Centroide
  Eigen::Vector3d centroid = SLAM_utility::fct_centroid(vec);

  //Covariance matrix
  Eigen::Matrix3d covMat = Eigen::Matrix3d::Zero();
  for(int i=0; i<vec.size(); i++){
    Eigen::Vector3d point = vec[i];

    for (int j=0; j<3; ++j){
      for (int k=j; k<3; ++k){
        covMat(j, k) += (point(j) - centroid(j)) * (point(k) - centroid(k));
      }
    }
  }
  covMat(1, 0) = covMat(0, 1);
  covMat(2, 0) = covMat(0, 2);
  covMat(2, 1) = covMat(1, 2);

  //---------------------------
  return covMat;
}
void make_translation(vector<vec3>& XYZ, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //Apply
  SLAM_utility::make_Transformation_atomic(XYZ, vec3(0, 0, 0), translation);

  //---------------------------
}
void make_rotation_origin(vector<vec3>& XYZ, mat4 R){
  //---------------------------

  vec3 COM = vec3(0, 0, 0);
  SLAM_utility::make_Transformation_atomic(XYZ, COM, R);

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
bool fct_is_nan(glm::vec3 vec){
  //---------------------------

  if(isnan(vec[0]) || isnan(vec[1]) || isnan(vec[2])){
    return true;
  }

  //---------------------------
  return false;
}
bool fct_is_nan(Eigen::Vector3d vec){
  //---------------------------

  if(isnan(vec(0)) || isnan(vec(1)) || isnan(vec(2))){
    return true;
  }

  //---------------------------
  return false;
}

}
