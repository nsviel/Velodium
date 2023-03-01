#ifndef SUBSET_BASE_H
#define SUBSET_BASE_H

#include "Cloud_base.h"


class Subset_base : public Cloud_base
{
public:
  //Constructor / Destructor
  Subset_base();
  ~Subset_base();

protected:
  int ID;
  Eigen::Matrix3d pose_R;
  Eigen::Vector3d pose_T;

  Frame frame;
  Data_icp icp;
  Data_pred detection;
  Data_pred detection_gt;
};

#endif
