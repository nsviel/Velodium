#include "SLAM_normal.h"

#include "../../../Specific/fct_maths.h"

#include <Eigen/Eigenvalues>


//Constructor / Destructor
SLAM_normal::SLAM_normal(){
  //---------------------------

  this->size_voxelMap = 1;
  this->nb_thread = 8;

  //---------------------------
}
SLAM_normal::~SLAM_normal(){}

//Main function
void SLAM_normal::compute_frameNormal(Frame* frame, voxelMap* map){
  int size = frame->xyz.size();
  //---------------------------

  //Reset variable conteners
  Nxyz.clear(); Nxyz.resize(size);
  NN.clear(); NN.resize(size);
  a2D.clear(); a2D.resize(size);

  //Compute normal for each point
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){

    // Neighborhood search
    Eigen::Vector3d point = frame->xyz[i];

    vector<Eigen::Vector3d> kNN = compute_kNN_search(point, map);

    //Compute kNN normals
    if(kNN.size() != 0){
      NN[i] = kNN[0];

      this->compute_normal(kNN, i);

      Eigen::Vector3d& point_raw = frame->xyz_raw[i];
      Eigen::Vector3d& normal = Nxyz[i];
      Eigen::Vector3d& trans_b = frame->trans_b;
      if(normal.dot(trans_b - point_raw) < 0){
        normal = -1.0 * normal;
      }

    }
  }

  if(Nxyz.size() == frame->xyz.size()){
    //Reoriente all normal
    this->compute_normals_reorientToOrigin(frame);

    //Store data
    frame->Nptp = Nxyz;
    frame->NN = NN;
    frame->a2D = a2D;
  }

  //---------------------------
}

//Sub function
vector<Eigen::Vector3d> SLAM_normal::compute_kNN_search(Eigen::Vector3d& point, voxelMap* map){
  priority_queue_iNN priority_queue;
  //---------------------------

  //Parameters
  int voxel_searchSize = 1;
  int max_number_neighbors = 20;
  int threshold_voxel_capacity = 1;

  int vx = static_cast<int>(point[0] / size_voxelMap);
  int vy = static_cast<int>(point[1] / size_voxelMap);
  int vz = static_cast<int>(point[2] / size_voxelMap);

  //Search inside all surrounding voxels
  for (int vi = vx - voxel_searchSize; vi <= vx + voxel_searchSize; vi++){
    for (int vj = vy - voxel_searchSize; vj <= vy + voxel_searchSize; vj++){
      for (int vk = vz - voxel_searchSize; vk <= vz + voxel_searchSize; vk++){

        //Search for pre-existing voxel in local map
        string voxel_id = to_string(vi) + " " + to_string(vj) + " " + to_string(vk);

        //If we found something
        if (map->find(voxel_id) != map->end()){
          vector<Eigen::Vector3d>& voxel_ijk = map->find(voxel_id).value();

          for (int i=0; i < voxel_ijk.size(); i++) {

            Eigen::Vector3d neighbor = voxel_ijk[i];
            float distance = (neighbor - point).norm();

            //If the voxel is full
            if (priority_queue.size() == max_number_neighbors) {
              if (distance < std::get<0>(priority_queue.top())) {
                priority_queue.pop();
                priority_queue.emplace(distance, neighbor);
              }
            } else{
              priority_queue.emplace(distance, neighbor);
            }
          }

        }

      }
    }
  }

  //Retrieve the kNN of the query point
  auto size = priority_queue.size();
  vector<Eigen::Vector3d> kNN(size);
  for(int i=0; i<size; i++){
    kNN[size - 1 - i] = std::get<1>(priority_queue.top());
    priority_queue.pop();
  }

  //---------------------------
  return kNN;
}
void SLAM_normal::compute_normal(vector<Eigen::Vector3d>& kNN, int i){
  // Computes normal and planarity coefficient
  //---------------------------

  //Compute normales
  Eigen::Matrix3d covMat = fct_covarianceMat(kNN);
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(covMat);
  Eigen::Vector3d normal = es.eigenvectors().col(0).normalized();
  this->Nxyz[i] = normal;

  // Compute planarity coefficient / weight from the eigen values
  //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
  float sigma_1 = sqrt(std::abs(es.eigenvalues()[2]));
  float sigma_2 = sqrt(std::abs(es.eigenvalues()[1]));
  float sigma_3 = sqrt(std::abs(es.eigenvalues()[0]));
  float a2D = (sigma_2 - sigma_3) / sigma_1;
  this->a2D[i] = a2D;

  //---------------------------
}
void SLAM_normal::compute_normals_reorientToOrigin(Frame* frame){
  vector<Eigen::Vector3d>& XYZ = frame->xyz;
  //---------------------------

  float dist_XYZ, dist_N;
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<XYZ.size(); i++){
    Eigen::Vector3d origine = Eigen::Vector3d::Zero();

    dist_XYZ = fct_distance(XYZ[i], origine);
    dist_N = fct_distance(XYZ[i] + Nxyz[i], origine);

    if(dist_N > dist_XYZ){
      for(int j=0; j<3; j++){
        Nxyz[i](j) = -Nxyz[i](j);
      }
    }
  }

  //---------------------------
}
