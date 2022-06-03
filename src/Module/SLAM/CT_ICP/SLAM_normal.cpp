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
  frame->Nptp.clear(); frame->Nptp.resize(frame->xyz.size());
  frame->NN.clear(); frame->NN.resize(frame->xyz.size());
  frame->a2D.clear(); frame->a2D.resize(frame->xyz.size());

  //Compute all point normal
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    vector<Eigen::Vector3f> kNN = compute_kNN_search(frame->xyz[i], map);
    this->compute_knn_normal(frame, kNN, i);
    this->compute_normal_reorientToOrigin(frame, i);
  }

  //---------------------------
}

//Sub function
vector<Eigen::Vector3f> SLAM_normal::compute_kNN_search(Eigen::Vector3f& point, voxelMap* map){
  priority_queue_iNN priority_queue;
  //---------------------------

  int vx = static_cast<int>(point[0] / knn_voxel_capacity);
  int vy = static_cast<int>(point[1] / knn_voxel_capacity);
  int vz = static_cast<int>(point[2] / knn_voxel_capacity);

  //Search inside all surrounding voxels
  int cpt = 0;
  for (int vi = vx - knn_voxel_search; vi <= vx + knn_voxel_search; vi++){
    for (int vj = vy - knn_voxel_search; vj <= vy + knn_voxel_search; vj++){
      for (int vk = vz - knn_voxel_search; vk <= vz + knn_voxel_search; vk++){
        //Search for pre-existing voxel in local map
        int key = (vi*200 + vj)*100 + vk;
        it_voxelMap it = map->find(key);

        //If we found a voxel with at least one point
        if(it != map->end()){
          vector<Eigen::Vector3f>& voxel_ijk = it.value();

          //We store all NN voxel point
          for(int i=0; i<voxel_ijk.size(); i++){
            Eigen::Vector3f& nn = voxel_ijk[i];
            float dist = (nn - point).norm();

            //If the voxel is full
            if(priority_queue.size() == knn_max_nn){
              //Raplace farest point
              float dist_last = std::get<0>(priority_queue.top());
              if(dist < dist_last){
                priority_queue.pop();
                priority_queue.emplace(dist, nn);
              }
            }
            //Else add point
            else{
              priority_queue.emplace(dist, nn);
            }
          }
        }
      }
    }
  }

  //Retrieve the kNN of the query point
  int size = priority_queue.size();
  vector<Eigen::Vector3f> kNN(size);
  for(int i=0; i<size; i++){
    kNN[size - 1 - i] = std::get<1>(priority_queue.top());
    priority_queue.pop();
  }

  //---------------------------
  return kNN;
}
void SLAM_normal::compute_knn_normal(Frame* frame, vector<Eigen::Vector3f>& kNN, int i){
  // Computes normal and planarity coefficient
  //---------------------------

  if(kNN.size() != 0){
    //NN point
    frame->NN[i] = kNN[0];

    //Compute normales
    Eigen::Matrix3f covMat = fct_covarianceMat(kNN);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> es(covMat);
    Eigen::Vector3f normal = es.eigenvectors().col(0).normalized();
    if(isnan(normal(0))){
      frame->Nptp[i] = Eigen::Vector3f::Zero();
    }else{
      frame->Nptp[i] = normal;
    }

    // Compute planarity coefficient / weight from the eigen values
    //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
    float sigma_1 = sqrt(std::abs(es.eigenvalues()[2]));
    float sigma_2 = sqrt(std::abs(es.eigenvalues()[1]));
    float sigma_3 = sqrt(std::abs(es.eigenvalues()[0]));
    float a2D = (sigma_2 - sigma_3) / sigma_1;
    if(isnan(a2D)){
      frame->a2D[i] = -1;
    }else{
      frame->a2D[i] = a2D;
    }
  }

  //---------------------------
}
void SLAM_normal::compute_normal_reorientToOrigin(Frame* frame, int i){
  Eigen::Vector3f& point = frame->xyz[i];
  Eigen::Vector3f& normal = frame->Nptp[i];
  //---------------------------

  if(normal.dot(frame->trans_b - point) < 0){
    normal = -1.0 * normal;
  }

  //---------------------------
}
