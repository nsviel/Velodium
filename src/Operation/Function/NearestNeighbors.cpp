#include "NearestNeighbors.h"

#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_transtypage.h"


//Constructor / Destructor
NearestNeighbors::NearestNeighbors(){}
NearestNeighbors::~NearestNeighbors(){}

MatrixXf NearestNeighbors::KdTreeFLANN(vector<vec2> PT_trg, vector<vec2> PT_query, bool normalized){
  int nb_iter = 128;
  int k = 30;
  MatrixXf list_kNN(PT_query.size(), k);
  //---------------------------

  if(normalized){
    //Min max
    float R_min = PT_trg[0][0];
    float R_max = PT_trg[0][0];
    float It_min = PT_trg[0][1];
    float It_max = PT_trg[0][1];

    for(int i=0; i<PT_trg.size(); i++){
      float R = PT_trg[i][0];
      if(R > R_max) R_max = R;
      if(R < R_min) R_min = R;

      float It = PT_trg[i][1];
      if(It > It_max) It_max = It;
      if(It < It_min) It_min = It;
    }

    //Normalization
    for(int i=0; i<PT_trg.size(); i++){
      PT_trg[i][0] = (PT_trg[i][0] - R_min) / (R_max - R_min);
      PT_trg[i][1] = (PT_trg[i][1] - It_min) / (It_max - It_min);
    }

    //Query normalization
    for(int i=0; i<PT_query.size(); i++){
      PT_query[i][0] = (PT_query[i][0] - R_min) / (R_max - R_min);
      PT_query[i][1] = (PT_query[i][1] - It_min) / (It_max - It_min);
    }
  }

  //Create FLANN matrices input
  flann::Matrix<float> dataset = flann::Matrix<float>(&PT_trg[0][0], PT_trg.size(), 2);
  flann::Matrix<float> query = flann::Matrix<float>(&PT_query[0][0], PT_query.size(), 2);

  //Construct an randomized kd-tree index using 4 kd-trees
  flann::Index<flann::L2<float>> index(dataset, flann::KDTreeIndexParams(4));
  index.buildIndex();

  //Create index matrices output
  vector<vector<int>> indices(query.rows, vector<int>(k,0));
  vector<vector<float>> dists(query.rows, vector<float>(k,0));
  flann::SearchParams params;
  params.cores = 0;
  params.checks = nb_iter;
  index.knnSearch(query, indices, dists, k, params);

  //Create vector of index
  for(int i=0; i<indices.size(); i++){
    for(int j=0; j<k; j++){
      list_kNN(i,j) = indices[i][j];
    }
  }

  //---------------------------
  return list_kNN;
}
vector<int> NearestNeighbors::kNN_number(vector<vec3>& PS, float pt_A, float pt_R){
  vector<int> list_kNN;
  vector<float> R, A, It;
  int k = 30;
  int PS_size = PS.size();
  //---------------------------

  //Normalization
  {
    float min, max;
    for(int i=0; i<PS_size; i++){
      R.push_back(PS[i].z);
      A.push_back(PS[i].y);
      It.push_back( acos(PS[i].y) * 180 / M_PI );
    }
    //R norm
    min = R[0]; max = R[0];
    for(int i=0; i<R.size(); i++){
      if(R[i] > max) max = R[i];
      if(R[i] < min) min = R[i];
    }
    pt_R = (pt_R - min) / (max - min);
    for(int i=0; i<R.size(); i++){
      R[i] = (R[i] - min) / (max - min);
    }
    //It norm
    min = It[0]; max = It[0];
    for(int i=0; i<A.size(); i++){
      if(It[i] > max) max = It[i];
      if(It[i] < min) min = It[i];
    }
    pt_A = (acos(pt_A) * 180 / M_PI );
    pt_A = (pt_A - min) / (max - min);
    for(int i=0; i<A.size(); i++){
      It[i] = (It[i] - min) / (max - min);
    }
  }

  //kNN
  vector<float> distance;
  for(int i=0; i<PS_size; i++){
    distance.push_back(sqrt( pow(R[i] - pt_R, 2) + pow(It[i] - pt_A, 2) ));
  }
  vector<size_t> idx = fct_sortByIndexes(distance);
  for(int i=0; i<k; i++){
    list_kNN.push_back(idx[i]);
  }

  //---------------------------
  if(list_kNN.size() == 0){
    cout<<"list kNN size is 0"<<endl;
  }
  return list_kNN;
}
vector<int> NearestNeighbors::kNN_radius(vector<vec3>& PS, float cosIt, float dist){
  vector<int> list_kNN_R, list_kNN_cIt;
  float lim_R = 2;
  float lim_cosIt = 0.2;
  int PS_size = PS.size();
  //---------------------------

  //kNN - R
  for(int i=0; i<PS_size; i++){
    float distance = sqrt(pow(PS[i].z - dist,2));

    if(distance < lim_R){
      list_kNN_R.push_back(i);
    }
  }

  //kNN - cosIt
  for(int i=0; i<list_kNN_R.size(); i++){
    float distance = sqrt(pow(PS[list_kNN_R[i]].y - cosIt,2));

    if(distance < lim_cosIt){
      list_kNN_cIt.push_back(list_kNN_R[i]);
    }
  }

  //---------------------------
  if(list_kNN_cIt.size() == 0){
    cout<<"list kNN size is 0"<<endl;
  }
  return list_kNN_cIt;
}
int NearestNeighbors::sequentialSearch(vector<vec3>& PS, float cosIt, float R){
  int idxR_low, idxR_up, idx_search, idx_median;
  int cpt = 0;
  bool ok = false;
  int PS_size = PS.size();
  idx_search = (int)PS_size/2;
  idx_median = (int)PS_size/2;
  //---------------------------

  for(int i=0; i<PS_size; i++){
    float PS_R = PS[i].z;

    if(R < PS_R){
      idxR_low = i-1;
      idxR_up = i;
    }
  }

  //---------------------------
  return 0;
}
int NearestNeighbors::binarySearch(vector<vec3>& PS, float cosIt, float R){
  int idxR_low, idxR_up, idx_search, idx_median;
  int cpt = 0;
  bool ok = false;
  int PS_size = PS.size();
  idx_search = (int)PS_size/2;
  idx_median = (int)PS_size/2;
  //---------------------------

  while(ok == false || cpt <= 10){
    float PS_R = PS[idx_search].z;

    if(R < PS_R){
      idx_search = (int)idx_median/2;
    }else if(R > PS_R){
      idx_search = idx_search + (int)idx_median/2;
    }else if(R == PS_R){
      ok = true;
    }

    idx_median = (int)idx_median/2;
    cpt++;
  }

  //---------------------------
  return 0;
}
