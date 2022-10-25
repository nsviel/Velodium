#ifndef Transforms_H
#define Transforms_H

#include "../../common.h"

class Transforms
{
public:
  //Constructor / Destructor
  Transforms();
  ~Transforms();

public:
  //Transformation
  void make_translation(Cloud* cloud, vec3 trans);
  void make_rotation(Cloud* cloud, vec3 COM, vec3 angles);
  void make_scaling(Cloud* cloud, float scaling);

  void make_translation(Subset* subset, vec3 trans);
  void make_rotation(Subset* subset, vec3 COM, vec3 angles);
  void make_scaling(Subset* subset, float scaling);

  void make_translation(vector<vec3>& XYZ, vec3 trans);
  void make_rotation(vector<vec3>& XYZ, vec3 radian);
  void make_rotation_origin(vector<vec3>& XYZ, mat4 R);

  void make_Transformation(Subset* subset, vec3 COM, mat4 transfMat);
  void make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation);
  void make_Transformation_point(vec3& XYZ, vec3 COM, mat4 Transformation);
  void make_Transformation_normal(vector<vec3>& N, mat4 Transformation);

  void make_cloud_rotation(Cloud* cloud, vec3 R, string direction);

  //Specific transformation functions
  void make_centering(Cloud* cloud);
  float make_orientAxis_X(Cloud* cloud);
  float make_alignAxisX_AB(Cloud* cloud, vec3 A, vec3 B);
  float make_alignAxisY_AB(Cloud* cloud, vec3 A, vec3 B);
  void make_elevation(Cloud* cloud, float Z);
  void make_alignAxis_X(Cloud* cloud);

  //Positionning functions
  void make_positionning(Cloud* cloud, vec3 pos);
  void make_positionning_XY(Cloud* cloud, vec3 pos);
  void make_positionning_glyph(vector<vec3>& XYZ, vec3& COM, vec3 pos);

  //Operations
  void fct_adjustPosToScanner(Cloud* cloud, float Z_scan);
  float fct_soilDetermination(Cloud* cloud);
  vec3 fct_degreeToRadian(vec3 degree);
  mat4 compute_transformMatrix(Subset* subset, vec3 COM, mat4 transformation);
  mat4 compute_transformMatrix(float tx, float ty, float tz, float rx, float ry, float rz);
  mat4 compute_transformMatrix(vec3 trans, vec3 rotat, vec3 scale);
  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);
  vec3 compute_anglesFromTransformationMatrix(const Eigen::Matrix3f& mat);
  vec3 compute_anglesFromTransformationMatrix(const Eigen::Matrix3d& mat);
  vec3 compute_translFromTransformationMatrix(const mat4& mat);
  vec3 compute_translationsError(Cloud* cloud);
  vec3 compute_anglesError(Cloud* cloud);
  vector<vec3> compute_transformcloud_XYZ(Cloud* cloud, mat4 Mat);
  void compute_transformXYZ(vector<vec3>& XYZ, vec3& COM, mat4 Mat);
  void compute_COM(Cloud* cloud);

  //Setters / Getters
  inline void set_soilnb_point(int value){this->soilnb_point = value;}

private:
  int soilnb_point;
};

#endif
