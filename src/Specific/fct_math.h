#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <numeric>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../common.h"

/**
 * \brief Basic math functions
 */


//Distance functions
float fct_distance(glm::vec3 pt1, glm::vec3 pt2);
float fct_distance_origin(Eigen::Vector3f pt1);
float fct_distance_origin(glm::vec3 pt1);
float fct_distance(Eigen::Vector3f pt1, Eigen::Vector3f pt2);
float fct_dotProduct(glm::vec3 vec_A, glm::vec3 vec_B);
float fct_sum(std::vector<float>& vec);
double fct_distance_origin(Eigen::Vector3d pt1);
double fct_distance(Eigen::Vector3d pt1, Eigen::Vector3d pt2);
double fct_distance(Eigen::Vector4d pt1, Eigen::Vector4d pt2);
double fct_distance(Eigen::Vector4d pt1, Eigen::Vector3d pt2);
bool fct_is_nan(glm::vec3 vec);
bool fct_is_nan(Eigen::Vector3d vec);

//Basic function
std::vector<float> fct_inv(std::vector<float>& vec);
std::vector<float> fct_ones(int size);
std::string thousandSeparator(int n);

//Minimum / Mean / Maximum
int fct_min_z_id(std::vector<glm::vec3> vec);
float fct_mean(std::vector<float>& vec);
float fct_max(std::vector<float>& vec);
float fct_max_vec(std::vector<std::vector<float>>& vec);
float fct_min(std::vector<float>& vec);
float fct_min(float in1, float in2);

Eigen::Vector3f fct_centroid(std::vector<Eigen::Vector3f>& XYZ);
Eigen::Vector3d fct_centroid(std::vector<Eigen::Vector3d>& XYZ);
glm::vec2 fct_min_vec2(std::vector<glm::vec2> XY);
glm::vec2 fct_max_vec2(std::vector<glm::vec2> XY);
glm::vec3 fct_centroid(std::vector<glm::vec3>& vec);
glm::vec3 fct_centroid(glm::vec3& vec1, glm::vec3& vec2);
glm::vec3 fct_max_vec3(std::vector<glm::vec3> XYZ);
glm::vec3 fct_min_z(std::vector<glm::vec3> vec);
glm::vec3 fct_min_vec3(std::vector<glm::vec3> XYZ);

//Normalization
std::vector<float> fct_normalize(std::vector<float>& vec);
std::vector<double> fct_normalize(std::vector<double>& vec);
std::vector<float> fct_normalize(std::vector<float>& vec, glm::vec2 range);
std::vector<float> fct_normalize(std::vector<float>& vec, float value_to_avoid);
std::vector<float> fct_normalize_01(std::vector<float>& vec);
std::vector<float> fct_crossProduct(std::vector<float>& vec_A, std::vector<float>& vec_B);
std::vector<float> fct_standardize(std::vector<float>& vec, float value_to_avoid);

//Statistical functions
float fct_std(std::vector<float>& vec);
float fct_var(std::vector<float>& vec);
float fct_cv(std::vector<float>& vec);
float fct_R2(std::vector<float>& data_X, std::vector<float>& data_Y, std::vector<float>& coeffs);
Eigen::Matrix3f fct_covarianceMat(std::vector<Eigen::Vector3f>& vec);
Eigen::Matrix3d fct_covarianceMat(std::vector<Eigen::Vector3d>& vec);

//Sorting functions
std::vector<size_t> fct_sortByIndexes(const std::vector<float> &v);
std::vector<size_t> fct_sortByIndexes_greater(const std::vector<float> &v);
bool is_number(const std::string& s);
void fct_sort_alpha_num_(std::vector<std::string>& vec);

//Geometric functions
float fct_oriented_angle(glm::vec2 A, glm::vec2 B);
float fct_degreeToRadian(float degree);
float fct_radianToDegree(float radian);
double fct_angularDistance(const Eigen::Matrix3f &rota, const Eigen::Matrix3f &rotb);
glm::vec3 fct_degreeToRadian_vec3(glm::vec3 degree);
glm::vec3 fct_radianToDegree_vec3(glm::vec3 radian);


#endif
