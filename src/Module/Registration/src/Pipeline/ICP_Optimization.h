#ifndef ICP_OPTIMIZATION_H
#define ICP_OPTIMIZATION_H

#include "../../../../common.h"
#include "../../../../Operation/Optimization/SpeudoInverse.h"


class ICP_Optimization
{
public:
  //Constructor / Destructor
  ICP_Optimization();
  ~ICP_Optimization();

public:
  void algo_Newton(Cloud* cloud_1, Cloud* cloud_2, vec3 COM);
  void algo_Newton_separated(Cloud* cloud_1, Cloud* cloud_2, vec3 COM);
  void algo_Xt(Cloud* cloud_P, Cloud* cloud_Q);
  void algo_Xr(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM);
  void algo_Xr_separated(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM);
  void init(int size);

  MatrixXf compute_Jacobian(vector<vec3>& XYZ_P, vec3 COM);
  MatrixXf compute_Jacobian_Xt();
  MatrixXf compute_Jacobian_Xr(vector<vec3>& XYZ_P, vec3 COM);
  MatrixXf compute_Jacobian_Xr_x(vector<vec3>& XYZ_P, vec3 COM);
  MatrixXf compute_Jacobian_Xr_y(vector<vec3>& XYZ_P, vec3 COM);
  MatrixXf compute_Jacobian_Xr_z(vector<vec3>& XYZ_P, vec3 COM);
  VectorXf compute_Errors(vector<vec3>& XYZ_P, vector<vec3>& XYZ_Q, vector<float>& key_w);
  VectorXf compute_Errors_Xr(vector<vec3>& XYZ_P, vector<vec3>& XYZ_Q, vector<float>& key_w);
  void compute_DOF(bool tx, bool ty, bool tz, bool Rx, bool Ry, bool Rz);
  void compute_param(VectorXf X);
  void compute_param_Pt(VectorXf Xt);
  void compute_param_Pr(Vector3f Xr);

  Matrix3f compute_dRotx(float theta);
  Matrix3f compute_dRoty(float theta);
  Matrix3f compute_dRotz(float theta);
  Matrix3f compute_Rotx(float theta);
  Matrix3f compute_Roty(float theta);
  Matrix3f compute_Rotz(float theta);

  inline void set_lambda(float value){this->lambda = value;}
  inline vec3 get_Pt(){return Pt;}
  inline vec3 get_Pr(){return Pr;}

private:
  SpeudoInverse invManager;
  Cloud* cloud_P_m;

  vector<bool> DOF;
  vec3 Pr, Pt;
  float lambda;
  int size, nP, nP_t, nP_r;
  VectorXf Xr_x, Xr_y, Xr_z;
};

#endif
