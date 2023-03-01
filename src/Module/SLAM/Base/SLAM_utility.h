#ifndef SLAM_UTILITY_H
#define SLAM_UTILITY_H

#include "common.h"

namespace SLAM_utility{

Eigen::Vector3d fct_centroid(std::vector<Eigen::Vector3d>& XYZ);
Eigen::Matrix3d fct_covarianceMat(std::vector<Eigen::Vector3d>& vec);
bool fct_is_nan(glm::vec3 vec);
bool fct_is_nan(Eigen::Vector3d vec);
void make_translation(vector<vec3>& XYZ, vec3 trans);
void make_rotation_origin(vector<vec3>& XYZ, mat4 R);
void make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation);

}



#endif
