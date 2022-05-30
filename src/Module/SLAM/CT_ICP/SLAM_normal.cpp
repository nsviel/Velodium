#include "SLAM_normal.h"

#include "../../../Specific/fct_maths.h"

#include <Eigen/Eigenvalues>


//Constructor / Destructor
SLAM_normal::SLAM_normal(){
  //---------------------------

  this->max_number_neighbors = 20;
  this->size_voxelMap = 1;
  this->voxel_searchSize = 1;
  this->nb_thread = 8;

  //---------------------------
}
SLAM_normal::~SLAM_normal(){}

//Main function
void SLAM_normal::compute_frameNormal(Frame* frame, voxelMap* map){
  //---------------------------

  //Reset variable conteners
  Nxyz.clear(); Nxyz.resize(frame->xyz.size());
  NN.clear(); NN.resize(frame->xyz.size());
  a2D.clear(); a2D.resize(frame->xyz.size());

  //Compute all point normal
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    vector<Eigen::Vector3f> kNN = compute_kNN_search(frame->xyz[i], map);
    this->compute_normal(kNN, i);
  }

  //Store data
  frame->Nptp = Nxyz;
  frame->NN = NN;
  frame->a2D = a2D;

  //Reoriente all normal
  this->compute_normals_reorientToOrigin(frame);

  //---------------------------
}

//Sub function
vector<Eigen::Vector3f> SLAM_normal::compute_kNN_search(Eigen::Vector3f& point, voxelMap* map){
  priority_queue_iNN priority_queue;
  //---------------------------

  int vx = static_cast<int>(point[0] / size_voxelMap);
  int vy = static_cast<int>(point[1] / size_voxelMap);
  int vz = static_cast<int>(point[2] / size_voxelMap);

  //Search inside all surrounding voxels
  int cpt = 0;
  for (int vi = vx - voxel_searchSize; vi <= vx + voxel_searchSize; vi++){
    for (int vj = vy - voxel_searchSize; vj <= vy + voxel_searchSize; vj++){
      for (int vk = vz - voxel_searchSize; vk <= vz + voxel_searchSize; vk++){

        //Search for pre-existing voxel in local map
        int key = (vi*200 + vj)*100 + vk;
        it_voxelMap it = map->find(key);

        //If we found a voxel with at least one point
        if(it != map->end()){
          vector<Eigen::Vector3f>& voxel_ijk = it.value();

          //We store all NN voxel point
          for(int i=0; i < voxel_ijk.size(); i++){
            float dist = (voxel_ijk[i] - point).norm();

            //If the voxel is full
            if(priority_queue.size() == max_number_neighbors){
              if(dist < std::get<0>(priority_queue.top())){
                priority_queue.pop();
                priority_queue.emplace(dist, voxel_ijk[i]);

                //If we have made lot of emplace, break loop
                //Could be source of errors (!)
                cpt++;
                if(cpt > 30){
                  break;
                }
              }
            }else{
              priority_queue.emplace(dist, voxel_ijk[i]);
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
void SLAM_normal::compute_normal(vector<Eigen::Vector3f>& kNN, int i){
  // Computes normal and planarity coefficient
  //---------------------------

  if(kNN.size() != 0){
    //NN point
    this->NN[i] = kNN[0];

    //Compute normales
    Eigen::Matrix3f covMat = fct_covarianceMat(kNN);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> es(covMat);
    Eigen::Vector3f normal = es.eigenvectors().col(0).normalized();
    this->Nxyz[i] = normal;

    // Compute planarity coefficient / weight from the eigen values
    //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
    float sigma_1 = sqrt(std::abs(es.eigenvalues()[2]));
    float sigma_2 = sqrt(std::abs(es.eigenvalues()[1]));
    float sigma_3 = sqrt(std::abs(es.eigenvalues()[0]));
    float a2D = (sigma_2 - sigma_3) / sigma_1;
    this->a2D[i] = a2D;
  }

  //---------------------------
}
void SLAM_normal::compute_normals_reorientToOrigin(Frame* frame){
  //---------------------------

  //Check vector size
  if(frame->Nptp.size() != frame->xyz.size()){
    cout<<"[SLAM] Normal size problem ("<<Nxyz.size()<<"|"<<frame->xyz.size()<<")"<<endl;
  }

  //Reoriente to origin
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3f& point = frame->xyz[i];
    Eigen::Vector3f& normal = frame->Nptp[i];

    if (normal.dot(frame->trans_b - point) < 0) {
        normal = -1.0 * normal;
    }
  }

  //---------------------------
}
