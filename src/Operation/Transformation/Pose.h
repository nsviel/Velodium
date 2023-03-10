#ifndef POSE_H
#define POSE_H

#include "../../common.h"

class Transformation;
class Fitting;


class Pose
{
public:
  //Constructor / Destructor
  Pose();
  ~Pose();

public:
    //Specific transformation functions
    void make_centering(Collection* collection);
    float make_orientAxis_X(Collection* collection);
    float make_alignAxisX_AB(Collection* collection, vec3 A, vec3 B);
    float make_alignAxisY_AB(Collection* collection, vec3 A, vec3 B);
    void make_elevation(Collection* collection, float Z);
    void make_alignAxis_X(Collection* collection);

    //Positionning functions
    void make_positionning(Collection* collection, vec3 pos);
    void make_positionning_XY(Collection* collection, vec3 pos);
    void make_positionning_glyph(vector<vec3>& XYZ, vec3& COM, vec3 pos);
    void make_alignSelectionToGround(Collection* collection);

    //Operations
    void make_adjustPosToScanner(Collection* collection, float Z_scan);
    float make_soilDetermination(Collection* collection);
    mat4 compute_transformMatrix(float tx, float ty, float tz, float rx, float ry, float rz);
    mat4 compute_transformMatrix(vec3 trans, vec3 rotat, vec3 scale);
    vec3 compute_anglesFromTransformationMatrix(const mat4& mat);
    vec3 compute_anglesFromTransformationMatrix(const Eigen::Matrix3f& mat);
    vec3 compute_anglesFromTransformationMatrix(const Eigen::Matrix3d& mat);
    vec3 compute_translFromTransformationMatrix(const mat4& mat);
    vec3 compute_translationsError(Collection* collection);
    vec3 compute_anglesError(Collection* collection);
    vector<vec3> compute_transformcloud_XYZ(Collection* collection, mat4 Mat);
    void compute_transformXYZ(vector<vec3>& XYZ, vec3& COM, mat4 Mat);
    void compute_COM(Collection* collection);
    void compute_min(Collection* collection);
    vec3 fct_degreeToRadian(vec3 degree);
    vec3 fct_radianToDegree(vec3 radian);

    //Setters / Getters
    inline void set_ground_nbPoint(int value){this->soilnb_point = value;}

private:
  Transformation* transformManager;
  Fitting* fittingManager;

  int soilnb_point;
};

#endif
