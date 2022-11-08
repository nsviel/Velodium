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
    void make_alignSelectionToGround(Cloud* cloud);

    //Operations
    void make_adjustPosToScanner(Cloud* cloud, float Z_scan);
    float make_soilDetermination(Cloud* cloud);
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
    void compute_min(Cloud* cloud);
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
