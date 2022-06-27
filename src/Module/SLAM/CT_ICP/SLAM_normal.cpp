#include "SLAM_normal.h"

#include "../../../Specific/fct_maths.h"

#include <Eigen/Eigenvalues>


//Constructor / Destructor
SLAM_normal::SLAM_normal(){
  //---------------------------

  //---------------------------
  this->update_configuration();
}
SLAM_normal::~SLAM_normal(){}

//Main function
void SLAM_normal::update_configuration(){
  //---------------------------

  this->knn_max_nn = 20;
  this->knn_voxel_capacity = 1;
  this->knn_voxel_search = 1;
  this->nb_thread = 8;

  //---------------------------
}
void SLAM_normal::compute_normal(Frame* frame, voxelMap* map){
  //---------------------------

  //Reset variable conteners
  frame->N_nn.clear(); frame->N_nn.resize(frame->xyz.size());
  frame->nn.clear(); frame->nn.resize(frame->xyz.size());
  frame->a2D.clear(); frame->a2D.resize(frame->xyz.size());

  //Compute all point normal
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    vector<Eigen::Vector3d> kNN = compute_kNN_search(frame->xyz[i], map);
    this->compute_knn_normal(frame, kNN, i);
    this->compute_normal_direction(frame, i);
  }

  //---------------------------
}

//Sub function
vector<Eigen::Vector3d> SLAM_normal::compute_kNN_search(Eigen::Vector3d& point, voxelMap* map){
  priority_queue_iNN priority_queue;
  int cpt = 0;
  //---------------------------

  int vx = static_cast<int>(point[0] / knn_voxel_capacity);
  int vy = static_cast<int>(point[1] / knn_voxel_capacity);
  int vz = static_cast<int>(point[2] / knn_voxel_capacity);

  //Search inside all surrounding voxels
  for (int vi = vx - knn_voxel_search; vi <= vx + knn_voxel_search; vi++){
    for (int vj = vy - knn_voxel_search; vj <= vy + knn_voxel_search; vj++){
      for (int vk = vz - knn_voxel_search; vk <= vz + knn_voxel_search; vk++){
        //Search for pre-existing voxel in local map
        int key = retrieve_map_signature(vi, vj, vk);
        voxelMap_it it = map->find(key);

        //If we found a voxel with at least one point
        if(it != map->end()){
          vector<Eigen::Vector3d>& voxel_ijk = it.value();

          //We store all NN voxel point
          for(int i=0; i<voxel_ijk.size(); i++){
            Eigen::Vector3d& nn = voxel_ijk[i];
            double dist = (nn - point).norm();

            //Fill the voxel until is full
            if(priority_queue.size() < knn_max_nn){
              priority_queue.emplace(dist, nn);
            }
            //Else replace farest point
            else{
              double dist_last = std::get<0>(priority_queue.top());
              if(dist < dist_last){
                priority_queue.pop();
                priority_queue.emplace(dist, nn);
              }
            }

          }
        }
      }
    }
  }

  //Retrieve the kNN of the query point
  int size = priority_queue.size();

  vector<Eigen::Vector3d> kNN(size);
  for(int i=0; i<size; i++){
    Eigen::Vector3d iNN = std::get<1>(priority_queue.top());
    kNN[size - 1 - i] = iNN;
    priority_queue.pop();
  }

  //---------------------------
  return kNN;
}
void SLAM_normal::compute_knn_normal(Frame* frame, vector<Eigen::Vector3d>& kNN, int i){
  // Computes normal and planarity coefficient
  //---------------------------

  //If no neighbor points
  if(kNN.size() == 0 || kNN.size() < 10){
    frame->N_nn[i] = Eigen::Vector3d::Zero();
    frame->N_nn[i](0) = NAN;
    frame->a2D[i] = NAN;
    frame->nn[i] = Eigen::Vector3d::Zero();
    frame->nn[i](0) = NAN;
    return;
  }

  //NN point
  frame->nn[i] = kNN[0];

  //Compute normales
  Eigen::Matrix3d covMat = fct_covarianceMat(kNN);
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(covMat);
  Eigen::Vector3d normal = es.eigenvectors().col(0).normalized();
  if(fct_is_nan(normal) == false){
    frame->N_nn[i] = normal;
  }else{
    frame->N_nn[i] = Eigen::Vector3d::Zero();
    frame->N_nn[i](0) = NAN;
  }

  // Compute planarity coefficient / weight from the eigen values
  double sigma_1 = sqrt(std::abs(es.eigenvalues()[2]));
  double sigma_2 = sqrt(std::abs(es.eigenvalues()[1]));
  double sigma_3 = sqrt(std::abs(es.eigenvalues()[0]));
  double a2D = (sigma_2 - sigma_3) / sigma_1;
  if(isnan(a2D) == false){
    frame->a2D[i] = a2D;
  }else{
    frame->a2D[i] = NAN;
  }

  //---------------------------
}
void SLAM_normal::compute_normal_direction(Frame* frame, int i){
  Eigen::Vector3d& point = frame->xyz[i];
  Eigen::Vector3d& normal = frame->N_nn[i];
  //---------------------------

  //Check for NaN
  if(isnan(normal(0))) return;

  if(normal.dot(frame->trans_b - point) < 0){
    normal = -1.0 * normal;
  }

  //---------------------------
}
