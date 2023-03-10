#include "SLAM_transform.h"
#include "SLAM_map.h"
#include "SLAM_sampling.h"
#include "SLAM.h"


#include "../Glyph/Object/Localmap.h"

#include "../../../Scene/Glyph/Object.h"
#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Specific/Function/fct_math.h"
#include "../../../Specific/Function/fct_transtypage.h"

#include <algorithm>


//Constructor / Destructor
SLAM_transform::SLAM_transform(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->slam_map = slam->get_slam_map();
  this->sceneManager = node_scene->get_sceneManager();
  this->objectManager = node_scene->get_objectManager();
  this->slam_sampling = slam->get_slam_sampling();

  //---------------------------
}
SLAM_transform::~SLAM_transform(){}

//Main function
void SLAM_transform::compute_preprocessing(Collection* collection, int subset_ID){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  Frame* frame = collection->get_frame_byID(subset_ID);
  //---------------------------

  slam_sampling->grid_sampling_subset(cloud);
  //this->distort_frame(frame);
  this->transform_frame(frame);

  //---------------------------
}

//Transformation functions
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
void SLAM_transform::transform_subset(Cloud* cloud){
  Frame* frame = &cloud->frame;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  //Update frame root
  cloud->rotat = eigen_to_glm_mat4(quat_b.toRotationMatrix());
  cloud->root = eigen_to_glm_vec3(trans_b);
  frame->is_slam_made = true;

  //Update cloud position
  //#pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<cloud->xyz.size(); i++){
    //Compute paramaters
    float ts_n = cloud->ts_n[i];
    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    //Apply transformation
    Eigen::Vector3d point {cloud->xyz[i].x, cloud->xyz[i].y, cloud->xyz[i].z};
    point = R * point + t;
    cloud->xyz[i] = vec3(point(0), point(1), point(2));
  }

  //Update cloud pose
  cloud->pose_T = frame->trans_b;
  cloud->pose_R = frame->rotat_b;
  cloud->root = vec3(frame->trans_b(0), frame->trans_b(1), frame->trans_b(2));

  //---------------------------
  sceneManager->update_buffer_location(cloud);
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
      frame->xyz[i] = quat_e_inv * (quat_n * frame->xyz[i] + t) + trans_e_inv;
    }
  }

  //---------------------------
}
