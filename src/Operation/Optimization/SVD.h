#ifndef SVD_h
#define SVD_h

#include "../../common.h"

class SVD
{
public:
  SVD();
  ~SVD();

public:
  void algo_SVD(vector<vec3>& XYZ_icp, vector<vec3>& XYZ_trg);
  void extractParameters();

private:
  vec3 transX, rotatX;
};

#endif
