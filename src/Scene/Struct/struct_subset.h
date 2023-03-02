#ifndef SUBSET_STRUCT_H
#define SUBSET_STRUCT_H

#include "struct_glyph.h"
#include "struct_frame.h"
#include "struct_data_pred.h"
#include "struct_data_icp.h"
#include "struct_object.h"


struct Subset : public Object_
{
  //---------------------------

  bool has_intensity;
  bool has_timestamp;

  //Main data
  std::vector<glm::vec3> xyz_voxel;
  std::vector<float> I;
  std::vector<double> ts_n;

  //Various attributs
  std::vector<float> A;
  std::vector<float> R;
  std::vector<float> cosIt;
  std::vector<float> It;
  std::vector<int> selected;
  std::list<int> highlighted;

  //Pose
  Eigen::Matrix3d pose_R;
  Eigen::Vector3d pose_T;

  //Own glyphs and structures
  std::map<std::string, Glyph> glyphs;
  Frame frame;
  Data_icp icp;
  Data_pred detection;
  Data_pred detection_gt;

  //---------------------------
};

#endif
