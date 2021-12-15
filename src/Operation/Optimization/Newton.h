#ifndef Newton_H
#define Newton_H

#include "SpeudoInverse.h"
#include "../../Engine/Data/struct_generic.h"
#include "../../common.h"

#include <Eigen/Dense>

using namespace Eigen;


class Newton
{
public:
  Newton();
  ~Newton();

public:
  //Newton methods
  void init(vector<vec3>& XYZ_1, vector<bool>& DOF);
  vector<float> algo_Newton(Cloud* cloud_P, Cloud* cloud_Q, vector<bool>& DOF, vector<Uplet> idx);
  vector<float> extractParameters(vector<bool>& DOF);

  //Math functions
  MatrixXf compute_Jacobian(vector<vec3>& XYZ_1, vector<bool>& DOF);
  VectorXf compute_Errors(vector<vec3>& XYZ_1, vector<vec3>& XYZ_2, vector<Uplet> idx);
  VectorXf vectorization(MatrixXf inMat);

  Matrix3f compute_dRotx(float theta);
  Matrix3f compute_dRoty(float theta);
  Matrix3f compute_dRotz(float theta);
  Matrix3f compute_Rotx(float theta);
  Matrix3f compute_Roty(float theta);
  Matrix3f compute_Rotz(float theta);

  inline void set_lambda(float value){this->lambda = value;}

private:
  SpeudoInverse invManager;

  vector<float> X_out;
  vector<Uplet> idx;

  VectorXf X;
  float lambda;
  int nb_iter;
  int size;
  int nP;
};

#endif
