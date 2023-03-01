#ifndef SLAM_UTILITY_H
#define SLAM_UTILITY_H

#include "../common.h"


void make_translation(vector<vec3>& XYZ, vec3 trans);
void make_rotation_origin(vector<vec3>& XYZ, mat4 R);
void make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation);


#endif
