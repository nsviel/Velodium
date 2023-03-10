#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "../../common.h"


class Transformation
{
public:
  // Constructor / Destructor
  Transformation();
  ~Transformation();

public:
  // Translation
  void make_translation(Collection* collection, vec3 trans);
  void make_translation(Object_* object, vec3 trans);
  void make_translation(vector<vec3>& XYZ, vec3 trans);

  // Rotation
  mat4 make_rotation(Collection* collection, vec3 COM, vec3 angles);
  void make_rotation(Collection* collection, vec3 R, string direction);
  void make_rotation(Object_* object, vec3 COM, vec3 angles);
  void make_rotation(vector<vec3>& XYZ, vec3 radian);
  void make_rotation_origin(vector<vec3>& XYZ, mat4 R);

  // Scaling
  void make_scaling(Collection* collection, float scaling);
  void make_scaling(Object_* object, float scaling);

  // Transformation
  void make_Transformation(Object_* object, vec3 COM, mat4 transfMat);
  void make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation);
  void make_Transformation_point(vec3& XYZ, vec3 COM, mat4 Transformation);
  void make_Transformation_normal(vector<vec3>& N, mat4 Transformation);
  mat4 make_transformation_attribut(Object_* object, vec3 COM, mat4 transformation);

private:
};

#endif
