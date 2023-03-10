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
  float algo_ICP(Collection* cloud_P, Collection* cloud_Q);
  float algo_NDT(Collection* cloud_P, Collection* cloud_Q);
  float algo_GICP(Collection* cloud_P, Collection* cloud_Q);
  float algo_LUM(Collection* cloud_P, Collection* cloud_Q);
  float algo_4ptsCong(Collection* cloud_P, Collection* cloud_Q);

  void algo_NN_OctreePCL(Collection* cloud_P, Collection* cloud_Q);
  void algo_NN_KdTreePCL(Collection* cloud_P, Collection* cloud_Q);

  Matrix4f optimization_SVD(Collection* cloud_1, Collection* cloud_2);
  Matrix4f optimization_LM(Collection* cloud_1, Collection* cloud_2);
  Matrix4f optimization_DualQuaternion(Collection* cloud_1, Collection* cloud_2);

private:

};

#endif
#endif
