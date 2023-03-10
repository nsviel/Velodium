#include "SLAM_sampling.h"
#include "SLAM.h"

#include "../Glyph/Object/Localmap.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Scene/Glyph/Object.h"
#include "../../../Specific/Function/fct_math.h"
#include "../../../Specific/Function/fct_transtypage.h"

#include <algorithm>


//Constructor / Destructor
SLAM_sampling::SLAM_sampling(){
  //---------------------------

  this->min_root_distance = 3.0f;
  this->max_root_distance = 100.0f;
  this->grid_voxel_width = 0.3;
  this->max_keypoint = 2000;

  //---------------------------
}
SLAM_sampling::~SLAM_sampling(){}

//Sampling functions
vector<vec3> SLAM_sampling::sub_sampling_subset(Cloud* cloud, float voxel_size){
  vector<vec3> xyz_sampled;
  float size_voxel = 0.2;
  //---------------------------

  cloudMap grid;
  Eigen::Vector4d point;
	for (int i=0; i<cloud->xyz.size(); i++) {
    int kx = static_cast<int>(cloud->xyz[i].x / voxel_size);
    int ky = static_cast<int>(cloud->xyz[i].y / voxel_size);
    int kz = static_cast<int>(cloud->xyz[i].z / voxel_size);
    int key = (kx*2000 + ky)*1000 + kz;
    point << cloud->xyz[i].x, cloud->xyz[i].y, cloud->xyz[i].z, 0;
    grid[key].push_back(point);
	}

  cloudMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++){
		if(it->second.size() > 0){
      Eigen::Vector4d point = it->second[0];
      vec3 xyz = vec3(point(0), point(1), point(2));
			xyz_sampled.push_back(xyz);
		}
	}

  //---------------------------
  return xyz_sampled;
}
void SLAM_sampling::grid_sampling_subset(Cloud* cloud){
  Frame* frame = &cloud->frame;
  //---------------------------

  //Clear vectors
  frame->xyz.clear();
  frame->xyz_raw.clear();
  frame->ts_n.clear();
  if(grid_voxel_width == 0){
    grid_voxel_width = 0.00001;
  }

  //Subsample the scan with voxels
  cloudMap grid;
  Eigen::Vector4d point;
  for(int i=0; i<cloud->xyz.size(); i++){
    vec3 xyz = cloud->xyz[i];
    double ts_n = cloud->ts_n[i];
    double dist = fct_distance_origin(xyz);

    if(dist > min_root_distance && dist < max_root_distance){
      int kx = static_cast<int>(xyz.x / grid_voxel_width);
      int ky = static_cast<int>(xyz.y / grid_voxel_width);
      int kz = static_cast<int>(xyz.z / grid_voxel_width);
      int key = (kx*2000 + ky)*1000 + kz;

      if(key >= INT_MAX || key <= INT_MIN){
        cout<<"[error] Int min or max value for key"<<endl;
      }

      point << xyz.x, xyz.y, xyz.z, ts_n;
      grid[key].push_back(point);
    }
  }

  //Take one point inside each voxel
  cloudMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++){
    if(it->second.size() == 0){
      continue;
    }

    int rdm = rand() % it->second.size();
    Eigen::Vector4d point = it->second[rdm];
    Eigen::Vector3d xyz(point(0), point(1), point(2));
    float ts_n = point(3);

    frame->xyz.push_back(xyz);
    frame->ts_n.push_back(ts_n);

    if(frame->xyz.size() >= max_keypoint){
      break;
    }
  }

  //frame->ts_n = fct_normalize(frame->ts_n);

  //---------------------------
  frame->xyz_raw = frame->xyz;
}
