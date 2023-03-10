#ifndef ICP_REJECTION_H
#define ICP_REJECTION_H

#include "../../struct_misc.h"
#include "../../../../common.h"


class ICP_Rejection
{
public:
  //Constructor / Destructor
  ICP_Rejection();
  ~ICP_Rejection();

public:
  //ICCP implementation
  void weighting_ICCP(Collection* cloud_P, Collection* cloud_Q);

  //Rejection methods
  void rejection_distance(Collection* cloud_P, Collection* cloud_Q, float threshold);
  void rejection_color(Collection* cloud_P, Collection* cloud_Q);
  void rejection_normal(Collection* cloud_P, Collection* cloud_Q);
  void rejection_duplicata(Collection* cloud_P, Collection* cloud_Q);

  //Subfunctions
  void make_supressPoints(vector<vec3>& XYZ, vector<int>& idx);

private:
  float thr_color;
};

#endif
