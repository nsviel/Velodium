#include "ICP_Matching.h"

#include <flann/flann.hpp>


//Constructor / Destructor
ICP_Matching::ICP_Matching(){}
ICP_Matching::~ICP_Matching(){}

//Simple matchibg
void ICP_Matching::algo_directMatching(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec3>& XYZ_P = subset_P->xyz;
  vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  //---------------------------

  if(XYZ_P.size() != XYZ_Q.size()){
    cout<<"Direct matching cloud sizes not equal"<<endl;
  }

  key_P = XYZ_P;
  trg_Q = XYZ_Q;

  //---------------------------*/
}
void ICP_Matching::algo_userSelection(Cloud* subset_P, Cloud* subset_Q){
  /*//Convert list<int> into vector<Uplet>
  list<int>& idx_1 = cloud_P->attribut.list_idxPoints;
  list<int>& idx_2 = cloud_Q->attribut.list_idxPoints;
  Uplet tuple;
  this->idx.clear();
  //---------------------------

  for(int i=0; i<idx_1.size(); i++){
    int id1 = *next(idx_1.begin(),i);
    int id2 = *next(idx_2.begin(),i);

    tuple = {cloud_P->ID, id1, cloud_Q->ID, id2};
    idx.push_back(tuple);
  }

  //---------------------------*/
}

//Geometric matching
void ICP_Matching::algo_NN_BruteForce(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec3>& XYZ_P = subset_P->xyz;
  vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  trg_Q.clear();
  //---------------------------

  key_P = XYZ_P;

  int id;
  for(int i=0; i<key_P.size(); i++){
    float min = 1000;
    for(int j=0; j<XYZ_Q.size(); j++){
      float dist = distance(key_P[i], XYZ_Q[j]);
      if(dist < min){
        min = dist;
        id = j;
      }
    }

    trg_Q.push_back(XYZ_Q[id]);
  }

  //---------------------------*/
}
void ICP_Matching::algo_NN_BruteForce_ICCP(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec3>& XYZ_P = subset_P->xyz;
  vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<float>& Is_P = cloud_P->intensity.OBJ;
  vector<float>& Is_Q = cloud_Q->intensity.OBJ;
  vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  key_P.clear(); trg_Q.clear();
  float Cmin = 0.99;
  //---------------------------

  vector<vec3> XYZ_Q_b;
  for(int i=0; i<XYZ_P.size(); i++){

    //Similarity measure
    XYZ_Q_b.clear();
    for(int j=0; j<XYZ_Q.size(); j++){
      float C = exp(-( abs(Is_P[i] - Is_Q[j]) )/0.2);

      if(C > Cmin){
        XYZ_Q_b.push_back(XYZ_Q[j]);
      }
    }

    // FLANN
    flann::Matrix<float> dataset = flann::Matrix<float>(&XYZ_Q_b[0][0], XYZ_Q_b.size(), 3);
    flann::Matrix<float> query = flann::Matrix<float>(&XYZ_P[i][0], 1, 3);
    flann::Index<flann::L2_3D<float>> index(dataset, flann::KDTreeSingleIndexParams(50));
    index.buildIndex();
    vector<vector<int>> indices(query.rows, vector<int>(1,0));
    vector<vector<float>> dists(query.rows, vector<float>(1,0));
    flann::SearchParams params;
    params.checks = 10;
    params.cores = 0;
    index.knnSearch(query, indices, dists, 1, params);

    //Output keypoints
    key_P.push_back(XYZ_P[i]);
    trg_Q.push_back(XYZ_Q_b[indices[0][0]]);
  }

  //---------------------------
  cout<<"there are "<< key_P.size() <<" keypoints"<<endl;
  cout<<"there are "<< trg_Q.size() <<" targets"<<endl;*/
}
void ICP_Matching::algo_NN_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<vec3>& key_P = subset_P->registration.keypoints;
  vector<vec3>& trg_Q = subset_Q->registration.trgpoints;
  //----------------------------

  //Create FLANN matrices input
  flann::Matrix<float> dataset = flann::Matrix<float>(&XYZ_Q[0][0], XYZ_Q.size(), 3);
  flann::Matrix<float> query = flann::Matrix<float>(&key_P[0][0], key_P.size(), 3);

  //Construct index
  flann::Index<flann::L2_3D<float>> index(dataset, flann::KDTreeSingleIndexParams(50));
  index.buildIndex();

  //Do knn search
  vector<vector<int>> indices(query.rows, vector<int>(1,0));
  vector<vector<float>> dists(query.rows, vector<float>(1,0));

  flann::SearchParams params;
  params.checks = 10;
  params.cores = 0;
  index.knnSearch(query, indices, dists, 1, params);

  //Output keypoints
  trg_Q.clear();
  for(int i=0; i<indices.size(); i++){
    int idx = indices[i][0];
    trg_Q.push_back(XYZ_Q[idx]);
  }

  //---------------------------*/
}

//Photometric matching
void ICP_Matching::algo_NI_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q){
  /*vector<float>& I_P = subset_P->I;
  vector<float>& I_Q = subset_Q->I;
  vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  int nb_iter = 10;
  //---------------------------

  //Create FLANN matrices input
  flann::Matrix<float> dataset = flann::Matrix<float>(&I_Q[0], I_Q.size(), 1);
  flann::Matrix<float> query = flann::Matrix<float>(&I_P[0], I_P.size(), 1);

  //Construct index
  flann::Index<flann::L2_3D<float>> index(dataset, flann::KDTreeSingleIndexParams(50));
  index.buildIndex();

  //Do knn search
  vector<vector<int>> indices(query.rows, vector<int>(1,0));
  vector<vector<float>> dists(query.rows, vector<float>(1,0));

  flann::SearchParams params;
  params.checks = nb_iter;
  params.cores = 0;
  index.knnSearch(query, indices, dists, 1, params);

  //Output keypoints
  trg_Q.clear();
  for(int i=0; i<indices.size(); i++){
    int idx = indices[i][0];
    trg_Q.push_back(vec4(XYZ_Q[idx], 1.0f));
  }

  //---------------------------*/
}
void ICP_Matching::algo_NI_BruteForce(Cloud* subset_P, Cloud* subset_Q){
  vector<float>& Is_icp = subset_P->I;
  vector<float>& Is_trg = subset_Q->I;
  if(Is_icp.size() == 0){cout<<"No intensity data"<<endl;}
  if(Is_trg.size() == 0){cout<<"No intensity data"<<endl;}
  this->idx.clear();
  //---------------------------

  int id;
  for(int i=0; i<Is_icp.size(); i++){
    float min = 1000;
    for(int j=0; j<Is_trg.size(); j++){
      float dist = sqrt(pow(Is_icp[i] - Is_trg[j], 2));
      if(dist < min){
        min = dist;
        id = j;
      }
    }
    Uplet tuple = {cloud_P->ID, i, cloud_Q->ID, id};
    idx.push_back(tuple);
  }

  //---------------------------
}

//Photometric + geometric matching
void ICP_Matching::algo_NI_NN(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec3>& XYZ_P = subset_P->xyz;
  vector<vec3>& XYZ_Q = subset_Q->xyz;
  vector<float>& Is_icp = cloud_P->intensity.OBJ;
  vector<float>& Is_trg = cloud_Q->intensity.OBJ;
  if(Is_icp.size() == 0){cout<<"No intensity data"<<endl;}
  if(Is_trg.size() == 0){cout<<"No intensity data"<<endl;}
  this->idx.clear();
  //---------------------------

  int id;
  float min;
  for(int i=0; i<XYZ_P.size(); i++){
    min = 1000;

    for(int j=0; j<XYZ_Q.size(); j++){
      float dist = sqrt(pow(Is_icp[i] - Is_trg[j], 2) + pow(XYZ_P[i].x - XYZ_Q[j].x, 2)
                 + pow(XYZ_P[i].y - XYZ_Q[j].y, 2) + pow(XYZ_P[i].z - XYZ_Q[j].z, 2));

      if(dist < min){
        min = dist;
        id = j;
      }
    }
    Uplet tuple = {cloud_P->ID, i, cloud_Q->ID, id};
    idx.push_back(tuple);
  }

  //---------------------------*/
}
void ICP_Matching::algo_NI_NN_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q){
  /*vector<vec4>& XYZI_P = cloud_P->registration.XYZI;
  vector<vec4>& XYZI_Q = cloud_Q->registration.XYZI;
  vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  vector<float>& key_I = cloud_P->registration.keypoint_intensity;
  vector<float>& trg_I = cloud_Q->registration.keypoint_intensity;
  int nb_iter = 10;
  //---------------------------

  //Create FLANN matrices input
  flann::Matrix<float> dataset = flann::Matrix<float>(&XYZI_Q[0][0], XYZI_Q.size(), 4);
  flann::Matrix<float> query = flann::Matrix<float>(&XYZI_P[0][0], XYZI_P.size(), 4);

  //Construct index
  flann::Index<flann::L2_Simple<float>> index(dataset, flann::KDTreeSingleIndexParams(50));
  index.buildIndex();

  //Do knn search
  vector<vector<int>> indices(query.rows, vector<int>(1,0));
  vector<vector<float>> dists(query.rows, vector<float>(1,0));

  flann::SearchParams params;
  params.checks = nb_iter;
  params.cores = 0;
  index.knnSearch(query, indices, dists, 1, params);

  //Output keypoints
  key_P.clear(); key_I.clear();
  trg_Q.clear(); trg_I.clear();
  for(int i=0; i<indices.size(); i++){
    int idx = indices[i][0];

    key_P.push_back(vec3(XYZI_P[i].x, XYZI_P[i].y, XYZI_P[i].z));
    key_I.push_back(XYZI_P[i][3]);

    trg_Q.push_back(vec3(XYZI_Q[idx].x, XYZI_Q[idx].y, XYZI_Q[idx].z));
    trg_I.push_back(XYZI_Q[idx][3]);
  }

  //---------------------------*/
}
void ICP_Matching::algo_NI_NN_KdTreeNanoFLANN(Cloud* subset_P, Cloud* subset_Q){
  /*/  vector<vec3>& XYZ_P = subset_P->xyz;
  vector<vec3>& XYZ_Q = subset_P->xyz;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  int nb_iter = 10;
  //---------------------------

  MatrixXf cloud_P = glm_to_eigen(XYZ_P);
  MatrixXf cloud_Q = glm_to_eigen(XYZ_Q);

  typedef Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> RowMatX3f;
  RowMatX3f TARGET = cloud_Q.leftCols(3);
  nanoflann::KDTreeEigenMatrixAdaptor<RowMatX3f> mat_index(3, TARGET, 50);

   mat_index.index->buildIndex();

   Eigen::MatrixXi indices;
   Eigen::MatrixXf dists;
   indices.resize(cloud_P.rows(), 1);
   dists.resize(cloud_P.rows(), 1);

   // do a knn search
   for (int i = 0; i < cloud_P.rows(); ++i) {
       // coords is RowMajor so coords.data()[i*3+0 / +1  / +2] represents the ith row of coords
       std::vector<float> query_pt{ cloud_P.data()[i*3+0], cloud_P.data()[i*3+1], cloud_P.data()[i*3+2] };

       std::vector<size_t> ret_indices(1);
       std::vector<float> out_dists_sqr(1);
       nanoflann::KNNResultSet<float> resultSet(1);
       resultSet.init(&ret_indices[0], &out_dists_sqr[0]);
       mat_index.index->findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(50));

       indices(i, 0) = ret_indices[0];
       dists(i, 0) = std::sqrt(out_dists_sqr[0]);
   }


   //----------------------------
   //Output keypoints
   trg_Q.clear();
   for(int i=0; i<indices.rows(); i++){
     int idx = indices(i,0);
     trg_Q.push_back(vec4(XYZ_Q[idx], 1.0f));
   }*/
}
