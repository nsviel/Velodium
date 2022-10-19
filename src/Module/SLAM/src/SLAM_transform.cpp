#include "SLAM_transform.h"

#include "SLAM_map.h"

#include "SLAM.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Object.h"
#include "../../../Engine/Scene/Object/SLAM/Localmap.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"


//Constructor / Destructor
SLAM_transform::SLAM_transform(SLAM* slam){
  //---------------------------

  Engine_node* node_engine = slam->get_node_engine();

  this->slam_map = slam->get_slam_map();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->with_distorsion = false;
  this->min_root_distance = 5.0f;
  this->max_root_distance = 100.0f;
  this->grid_voxel_width = 1;
  this->max_total_point = 20000;

  //---------------------------
}
SLAM_transform::~SLAM_transform(){}

//Main function
void SLAM_transform::compute_preprocessing(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  this->grid_sample_subset(subset);
  this->distort_frame(frame);
  //this->transform_frame(frame);

  //---------------------------
}

//Specific function
void SLAM_transform::grid_sample_subset(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Clear vectors
  frame->xyz.clear();
  frame->xyz_raw.clear();
  frame->ts_n.clear();

  //Subsample the scan with voxels
  gridMap grid;
  Eigen::Vector4d point;
  for(int i=0; i<subset->xyz.size(); i++){
    vec3 xyz = subset->xyz[i];
    double ts_n = subset->ts_n[i];
    double dist = fct_distance_origin(xyz);

    if(dist > min_root_distance && dist < max_root_distance){
      int kx = static_cast<int>(xyz.x / grid_voxel_width);
      int ky = static_cast<int>(xyz.y / grid_voxel_width);
      int kz = static_cast<int>(xyz.z / grid_voxel_width);
      int key = (kx*2000 + ky)*1000 + kz;

      point << xyz.x, xyz.y, xyz.z, ts_n;
      grid[key].push_back(point);
    }
  }

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); it++){
    if(it->second.size() != 0){
      //Take one random point
      int rdm = rand() % it->second.size();

      Eigen::Vector4d point = it->second[rdm];
      Eigen::Vector3d xyz(point(0), point(1), point(2));

      frame->xyz.push_back(xyz);
      frame->ts_n.push_back(point(3));

      if(frame->xyz.size() > max_total_point){
        break;
      }
    }
  }

  frame->xyz_raw = frame->xyz;

  //---------------------------
}
void SLAM_transform::distort_frame(Frame* frame){
  //---------------------------

  if(with_distorsion && frame->ID > 1){
    Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
    Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;

    //Distorts the frame
    Eigen::Quaterniond quat_e_inv = quat_e.inverse();
    Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e);

    for(int i=0; i<frame->xyz.size(); i++){
      float ts_n = frame->ts_n[i];

      Eigen::Quaterniond quat_n = quat_b.slerp(ts_n, quat_e).normalized();
      Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

      // Distort Raw Keypoints
      frame->xyz_raw[i] = quat_e_inv * (quat_n * frame->xyz_raw[i] + t) + trans_e_inv;
    }
  }

  //---------------------------
}
void SLAM_transform::transform_frame(Frame* frame){
  //---------------------------

  //Update keypoints
  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  #pragma omp parallel for num_threads(5)
  for(int i=0; i<frame->xyz.size(); i++){
    double ts_n = frame->ts_n[i];

    Eigen::Quaterniond q = quat_b.slerp(ts_n, quat_e);
    q.normalize();
    Eigen::Matrix3d R = q.toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    frame->xyz[i] = R * frame->xyz_raw[i] + t;
  }

  //---------------------------
}
void SLAM_transform::transform_subset(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;
  say(frame->rotat_b);

  //Update frame root
  subset->rotat = eigen_to_glm_mat4(quat_b.toRotationMatrix());
  subset->root = eigen_to_glm_vec3(trans_b);
  frame->is_slamed = true;

  //Update subset position
  //#pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<subset->xyz.size(); i++){
    //Compute paramaters
    float ts_n = subset->ts_n[i];
    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    //Apply transformation
    Eigen::Vector3d point {subset->xyz[i].x, subset->xyz[i].y, subset->xyz[i].z};
    point = R * point + t;
    subset->xyz[i] = vec3(point(0), point(1), point(2));
  }

  //---------------------------
  sceneManager->update_subset_location(subset);
}
void SLAM_transform::transform_glyph(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Update keypoint
  if(frame->xyz.size() == frame->nn.size()){
    vector<vec3> xyz;
    vector<vec3> Nxy;
    vector<float> ts;

    for(int i=0; i<frame->xyz.size(); i++){
      if(isnan(frame->nn[i](0)) == false){
        xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
        Nxy.push_back(vec3(frame->N_nn[i](0), frame->N_nn[i](1), frame->N_nn[i](2)));
        ts.push_back(frame->ts_n[i]);
      }
    }

    subset->keypoint.location = xyz;
    subset->keypoint.timestamp = ts;
    subset->keypoint.normal = Nxy;
  }

  //Update local map
  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(slam_map->get_local_map());
  objectManager->update_object(mapObject->get_glyph());

  //---------------------------
  objectManager->update_glyph_subset(subset);
}
void SLAM_transform::reset_glyph(){
  //---------------------------

  objectManager->reset_scene_object();

  //---------------------------
}
