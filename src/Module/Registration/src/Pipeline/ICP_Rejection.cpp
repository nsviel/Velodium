#include "ICP_Rejection.h"


//Constructor / Destructor
ICP_Rejection::ICP_Rejection(){
  //---------------------------

  this->thr_color = 0.3;

  //---------------------------
}
ICP_Rejection::~ICP_Rejection(){}

//ICCP implementation
void ICP_Rejection::weighting_ICCP(Cloud* cloud_P, Cloud* cloud_Q){
  /*vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  vector<float>& key_I = cloud_P->registration.keypoint_intensity;
  vector<float>& trg_I = cloud_Q->registration.keypoint_intensity;
  vector<float>& key_w = cloud_P->registration.keypoint_weight;
  vector<int> idx;

  float dmax = 0.1;
  key_w.clear();
  cout<<"-> weighting ICCP "<<flush;
  //---------------------------

  for(int i=0; i<key_I.size(); i++){
    float C = exp(-(key_I[i] - trg_I[i])/0.2);
    float d = distance(key_P[i], trg_Q[i]);

    if(d < dmax){
      float wi = C *(1 - d/dmax);
      key_w.push_back(wi);
    }else{
      idx.push_back(i);
    }
  }

  //---------------------------
  cout<<": Supression of "<<idx.size()<<" points"<<endl;
  this->make_supressPoints(key_P, idx);
  this->make_supressPoints(trg_Q, idx);*/
}

//Rejection methods
void ICP_Rejection::rejection_distance(Cloud* cloud_P, Cloud* cloud_Q, float threshold){
  /*vector<vec3>& XYZ_icp = cloud_P->registration.keypoints;
  vector<vec3>& XYZ_trg = cloud_Q->registration.trgpoints;
  vector<int> idx;
  cout<<"-> rejection distance: "<<XYZ_icp.size()<<flush;
  //---------------------------

  for(int i=0; i<XYZ_icp.size(); i++){
    float dist = distance(XYZ_icp[i], XYZ_trg[i]);

    if(dist > threshold) idx.push_back(i);
  }

  //---------------------------
  this->make_supressPoints(XYZ_icp, idx);
  this->make_supressPoints(XYZ_trg, idx);
  cout<<"->"<<XYZ_icp.size()<<endl;*/
}
void ICP_Rejection::rejection_color(Cloud* cloud_P, Cloud* cloud_Q){
  /*vector<vec4>& RGB_icp = cloud_P->color.OBJ;
  vector<vec4>& RGB_trg = cloud_Q->color.OBJ;
  Uplet tuple;
  //---------------------------

  for(int i=idx.size()-1; i>=0; i--){
    tuple = idx[i];
    float diff = 0;
    for(int j=0; j<3; j++) diff += RGB_icp[i][j] - RGB_trg[tuple.idx2][j];
    if(diff > thr_color) idx.erase(idx.begin()+i);
  }*/

  //---------------------------
}
void ICP_Rejection::rejection_normal(Cloud* cloud_P, Cloud* cloud_Q){
  /*vector<vec3>& n1 = cloud_P->normal.OBJ;
  vector<vec3>& n2 = cloud_Q->normal.OBJ;
  vector<vec3>& XYZ_icp = cloud_P->location.OBJ;
  vector<vec3>& XYZ_trg = cloud_Q->location.OBJ;
  Uplet tuple;
  //---------------------------

  for(int i=idx.size()-1; i>=idx.size()-3; i--){
    tuple = idx[i];
    float angle = 0;
    for(int j=0; j<3; j++)
      angle += (XYZ_icp[i][j] + n1[i][j]) * (XYZ_trg[tuple.idx2][j] + n2[tuple.idx2][j]);

      angle = acos(angle);
  }*/

  //---------------------------
}
void ICP_Rejection::rejection_duplicata(Cloud* cloud_P, Cloud* cloud_Q){
  vector<int> id;
  Uplet tuple1, tuple2;
  //---------------------------

  /*for(int i=0; i<idx.size(); i++){
    tuple1 = idx[i];
    for(int j=i+1; j<idx.size(); j++){
      tuple2 = idx[j];
      if(tuple2.idx2 == tuple1.idx2)
      {
        id.push_back(tuple2.idx2);
        break;
      }
    }
  }

  for(int i=idx.size()-1; i>=0; i--){
    tuple1 = idx[i];
    for(int j=0; j<id.size(); j++){
      if(id[j] == tuple1.idx2)
      {
        idx.erase(idx.begin()+i);
        break;
      }
    }
  }*/

  //---------------------------
}

//Subfunctions
void ICP_Rejection::make_supressPoints(vector<vec3>& XYZ, vector<int>& idx){
  if(idx.size() == 0)return;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<vec3> XYZ_b;
  int cpt = 0;

  for(int i=0; i<XYZ.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      XYZ_b.push_back(XYZ[i]);;
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //---------------------------
  XYZ = XYZ_b;
}
