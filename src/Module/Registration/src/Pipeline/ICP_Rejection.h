#ifndef ICP_REJECTION_H
#define ICP_REJECTION_H

#include "../../../../Engine/Data/struct_misc.h"
#include "../../../../common.h"


class ICP_Rejection
{
public:
  //Constructor / Destructor
  ICP_Rejection();
  ~ICP_Rejection();

public:
  //ICCP implementation
  void weighting_ICCP(Cloud* cloud_P, Cloud* cloud_Q);

  //Rejection methods
  void rejection_distance(Cloud* cloud_P, Cloud* cloud_Q, float threshold);
  void rejection_color(Cloud* cloud_P, Cloud* cloud_Q);
  void rejection_normal(Cloud* cloud_P, Cloud* cloud_Q);
  void rejection_duplicata(Cloud* cloud_P, Cloud* cloud_Q);

  //Subfunctions
  void make_supressPoints(vector<vec3>& XYZ, vector<int>& idx);

private:
  float thr_color;
};

#endif
