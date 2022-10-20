#ifdef WITH_PCL
#ifndef ALGO_PCL_H
#define ALGO_PCL_H

#include "../../Parameters.h"

class Algo_PCL
{
public:
  //Constructor / Destructor
  Algo_PCL();
  ~Algo_PCL();

public:
  float algo_ICP(Cloud* cloud_P, Cloud* cloud_Q);
  float algo_NDT(Cloud* cloud_P, Cloud* cloud_Q);
  float algo_GICP(Cloud* cloud_P, Cloud* cloud_Q);
  float algo_LUM(Cloud* cloud_P, Cloud* cloud_Q);
  float algo_4ptsCong(Cloud* cloud_P, Cloud* cloud_Q);

  void algo_NN_OctreePCL(Cloud* cloud_P, Cloud* cloud_Q);
  void algo_NN_KdTreePCL(Cloud* cloud_P, Cloud* cloud_Q);

  Matrix4f optimization_SVD(Cloud* cloud_1, Cloud* cloud_2);
  Matrix4f optimization_LM(Cloud* cloud_1, Cloud* cloud_2);
  Matrix4f optimization_DualQuaternion(Cloud* cloud_1, Cloud* cloud_2);

private:

};

#endif
#endif
