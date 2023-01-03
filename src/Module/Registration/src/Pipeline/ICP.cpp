#include "ICP.h"
#include "ICP_Matching.h"
#include "ICP_Rejection.h"

#include "../Correspondence/Keypoint.h"

#include "../../../../Operation/Transformation/Transformation.h"
#include "../../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../../Specific/fct_transtypage.h"


//Constructor / Destructor
ICP::ICP(Glyphs* glyph){
  //---------------------------

  this->glyphManager = glyph;
  this->transformManager = new Transformation();
  this->matchManager = new ICP_Matching();
  this->optManager = new ICP_Optimization();
  this->rejectManager = new ICP_Rejection();
  //this->keyManager = new Keypoint(glyphManager);
  //this->pclManager = new Algo_PCL();

  //---------------------------
  this->init();
}
ICP::~ICP(){}

//Main functions
void ICP::init(){
  //---------------------------

  //Parameters
  this->nD = 3; //Space dimension (1D, 2D, 3D)
  this->nP = 6; //Degree of freedom (1D->1, 2D->3, 3D->6)
  this->SSE = 0;
  this->SSE_old = 10;
  this->SSE_new = 10;
  this->gain = 1;
  this->match_percentPts = 100;
  this->makeCorrespondences = true;

  //Methods
  this->reject_normal = false;
  this->reject_dist = false;
  this->reject_dupli = false;
  this->rejection_distance_threshold = 0.3;

  //0 = geo/I
  //1 = keypoints
  //6 = direct matching
  this->m_match = 1;
  this->m_optim = 0;
  this->m_com = 0;

  //Weights
  this->Xt_w = vec3(1,1,1);
  this->Xr_w = vec3(1,1,1);

  //DOFs
  this->dof_tx = true;
  this->dof_ty = true;
  this->dof_tz = true;
  this->dof_Rx = true;
  this->dof_Ry = true;
  this->dof_Rz = true;

  //---------------------------
}
void ICP::reset(){
  //---------------------------

  this->Xt = vec3(0,0,0);
  this->Xr = vec3(0,0,0);

  this->Xt_w = vec3(1,1,1);
  this->Xr_w = vec3(1,1,1);

  this->SSE = 0;
  this->SSE_old = 10;
  this->SSE_new = 10;

  this->time_match = 0;
  this->time_reject = 0;
  this->time_opti = 0;

  this->makeCorrespondences = true;

  //---------------------------
}
void ICP::algo(Cloud* cloud_P, Cloud* cloud_Q){
  //---------------------------

  //Matching
  tic();
  this->step_matching(cloud_P, cloud_Q);
  SSE_old = compute_SSE(cloud_P, cloud_Q);
  time_match = toc();

  //Matching rejection
  tic();
  this->step_rejection(cloud_P, cloud_Q);
  time_reject = toc();

  //Optimization
  tic();
  this->step_optimization(cloud_P, cloud_Q);
  time_opti = toc();

  //Apply transformation
  this->step_transformation(cloud_P, cloud_Q);
  SSE_new = compute_SSE(cloud_P, cloud_Q);

  //---------------------------
}

//Step functions
void ICP::step_matching(Cloud* cloud_P, Cloud* cloud_Q){
  /*//---------------------------

  switch(m_match){
    case 0:{// Adaptative (XYZI/XYZ)
      this->compute_prepareData(cloud_P, cloud_Q);

      //Check for cloud intensity, else make geometric matching
      if(cloud_P->intensity.OBJ.size() != 0 && cloud_Q->intensity.OBJ.size() != 0){
        matchManager->algo_NI_NN_KdTreeFLANN(cloud_P, cloud_Q);
      }else{
        matchManager->algo_NN_KdTreeFLANN(cloud_P, cloud_Q);
      }
      break;
    }
    case 1:{// Keypoints
      if(makeCorrespondences || cloud_P->registration.keypoints.size() == 0){
        //keyManager->algo_keypoints(cloud_P, cloud_Q);
        makeCorrespondences = false;
      }
      break;
    }
    case 2:{// Intensity (XYZI)
      this->compute_prepareData(cloud_P, cloud_Q);
      matchManager->algo_NI_NN_KdTreeNanoFLANN(cloud_P, cloud_Q);
      break;
    }
    case 3:{// Geometric (XYZ)
      this->compute_prepareData(cloud_P, cloud_Q);
      matchManager->algo_NN_KdTreeFLANN(cloud_P, cloud_Q);
      break;
    }
    case 4:{// Pure intensity (I)
      matchManager->algo_NI_KdTreeFLANN(cloud_P, cloud_Q);
      break;
    }
    case 5:{// User selection
      matchManager->algo_userSelection(cloud_P, cloud_Q);
      break;
    }
    case 6:{// Direct matching
      matchManager->algo_directMatching(cloud_P, cloud_Q);
      break;
    }
    case 7:{// ICCP
      matchManager->algo_NN_BruteForce_ICCP(cloud_P, cloud_Q);
      break;
    }
  }

  //---------------------------*/
}
void ICP::step_rejection(Cloud* cloud_P, Cloud* cloud_Q){
  /*vector<float>& wi = cloud_P->registration.keypoint_weight;
  bool weighting_ICCP = false;
  wi.clear();
  //---------------------------

  //Prepare unity weight
  for(int i=0; i<cloud_P->registration.keypoints.size(); i++){
    wi.push_back(1);
  }

  //Rejection methods
  if(reject_dist){
    rejectManager->rejection_distance(cloud_P, cloud_Q, rejection_distance_threshold);
  }
  if(reject_normal){
    rejectManager->rejection_normal(cloud_P, cloud_Q);
  }
  if(reject_dupli){
    rejectManager->rejection_duplicata(cloud_P, cloud_Q);
  }
  if(weighting_ICCP){
    rejectManager->weighting_ICCP(cloud_P, cloud_Q);
  }

  //---------------------------*/
}
void ICP::step_optimization(Cloud* cloud_P, Cloud* cloud_Q){
  /*Xt = vec3(0,0,0);
  Xr = vec3(0,0,0);
  //--------------------------

  switch(m_optim){
    case 0:{ //Newton
      vec3 com;
      if(m_com == 0) com = cloud_P->location.COM;
      if(m_com == 1) com = cloud_P->location.root;

      optManager->compute_DOF(dof_tx, dof_ty, dof_tz, dof_Rx, dof_Ry, dof_Rz);
      optManager->algo_Newton_separated(cloud_P, cloud_Q, com);

      Xt = optManager->get_Pt();
      Xr = optManager->get_Pr();
      break;
    }
    case 1:{ //SVD pcl
      Matrix4f mat = pclManager->optimization_SVD(cloud_P, cloud_Q);
      mat4 transMat = eigen_to_glm_mat4(mat);
      transformManager->make_Transformation(cloud_P, vec3(0,0,0), transMat);
      break;
    }
    case 2:{ //LM pcl
      Matrix4f mat = pclManager->optimization_LM(cloud_P, cloud_Q);
      mat4 transMat = eigen_to_glm_mat4(mat);
      transformManager->make_Transformation(cloud_P, vec3(0,0,0), transMat);
      break;
    }
    case 3:{ //Dual Quaternion pcl
      Matrix4f mat = pclManager->optimization_DualQuaternion(cloud_P, cloud_Q);
      mat4 transMat = eigen_to_glm_mat4(mat);
      transformManager->make_Transformation(cloud_P, vec3(0,0,0), transMat);
      break;
    }
    case 4:{ //None
      break;
    }
  }

  //--------------------------*/
}
void ICP::step_transformation(Cloud* cloud_P, Cloud* cloud_Q){
  /*//---------------------------

  //Add weight to dofs
  for(int i=0; i<3; i++){
    Xt[i] = Xt[i] * Xt_w[i];
    Xr[i] = Xr[i] * Xr_w[i];
  }

  //Optimization center of mass
  vec3 com;
  if(m_com == 0) com = cloud_P->location.COM + Xt;
  if(m_com == 1) com = cloud_P->location.root + Xt;

  //Apply transformation
  transformManager->make_translation(cloud_P, Xt);
  transformManager->make_rotation(cloud_P, com, Xr);

  //---------------------------*/
}

//Subfunctions
float ICP::compute_SSE(Cloud* cloud_P, Cloud* cloud_Q){
  float SSE;
  /*vector<vec3>& key_P = cloud_P->registration.keypoints;
  vector<vec3>& trg_Q = cloud_Q->registration.trgpoints;
  //---------------------------

  if(key_P.size() == 0 || trg_Q.size() == 0){
    SSE = 10;
  }else{
    SSE = 0;
    for(int i=0; i<key_P.size(); i++){
      vec3 P(key_P[i]);
      vec3 Q(trg_Q[i]);
      SSE += pow(distance(P, Q), 2);
    }
    SSE = SSE / key_P.size();
  }*/

  //---------------------------
  return SSE;
}
float ICP::compute_SSE_groundTruth(Cloud* cloud){
  float SSE;
  /*vector<vec3>& XYZ = cloud->location.OBJ;
  vector<vec3>& XYZ_truth = cloud->registration.XYZ_groundTruth;
  //---------------------------

  if(XYZ.size() == 0 || XYZ_truth.size() == 0){
    SSE = 10;
    return SSE;
  }
  if(XYZ.size() != XYZ_truth.size()){
    cout<<"Ground truth error size problem !"<<endl;
  }

  SSE = 0;
  for(int i=0; i<XYZ.size(); i++){
    vec3 P(XYZ[i]);
    vec3 Pg(XYZ_truth[i]);
    SSE += pow(distance(P, Pg), 2);
  }*/

  //---------------------------
  return SSE;
}
float ICP::compute_MSE_groundTruth(Cloud* cloud){
  float MSE;
  /*vector<vec3>& XYZ = cloud->location.OBJ;
  vector<vec3>& XYZ_truth = cloud->registration.XYZ_groundTruth;
  float MSE;
  //---------------------------

  //Check data
  if(XYZ.size() == 0 || XYZ_truth.size() == 0){
    MSE = 10;
    return MSE;
  }
  if(XYZ.size() != XYZ_truth.size()){
    cout<<"Ground truth error size problem !"<<endl;
  }

  //MSE error
  MSE = 0;
  for(int i=0; i<XYZ.size(); i++){
    vec3 P(XYZ[i]);
    vec3 Pg(XYZ_truth[i]);
    MSE += pow(distance(P, Pg), 2);
  }
  MSE = MSE / XYZ.size();*/

  //---------------------------
  return MSE;
}
float ICP::compute_RMSE_groundTruth(Cloud* cloud){
  float RMSE;
  /*vector<vec3>& XYZ = cloud->location.OBJ;
  vector<vec3>& XYZ_truth = cloud->registration.XYZ_groundTruth;
  float RMSE;
  //---------------------------

  if(XYZ.size() == 0 || XYZ_truth.size() == 0){
    RMSE = 10;
    return RMSE;
  }
  if(XYZ.size() != XYZ_truth.size()){
    cout<<"Ground truth error size problem !"<<endl;
  }

  RMSE = 0;
  for(int i=0; i<XYZ.size(); i++){
    vec3 P(XYZ[i]);
    vec3 Pg(XYZ_truth[i]);
    RMSE += pow(distance(P, Pg), 2);
  }
  RMSE = RMSE / XYZ.size();
  RMSE = sqrt(RMSE);*/

  //---------------------------
  return RMSE;
}
float ICP::compute_MAE_groundTruth(Cloud* cloud){
  float MAE;
  /*vector<vec3>& XYZ = cloud->location.OBJ;
  vector<vec3>& XYZ_truth = cloud->registration.XYZ_groundTruth;
  //---------------------------

  if(XYZ.size() == 0 || XYZ_truth.size() == 0){
    MAE = 10;
    return MAE;
  }
  if(XYZ.size() != XYZ_truth.size()){
    cout<<"Ground truth error size problem !"<<endl;
  }

  MAE = 0;
  for(int i=0; i<XYZ.size(); i++){
    vec3 P(XYZ[i]);
    vec3 Pg(XYZ_truth[i]);
    MAE += abs(distance(P, Pg));
  }
  MAE = MAE / XYZ.size();*/

  //---------------------------
  return MAE;
}
void ICP::compute_prepareData(Cloud* cloud_P, Cloud* cloud_Q){
  /*//Prepare source data combining location and intensity for registration based intensity
  //---------------------------

  //Source cloud
  vector<vec3>& XYZ_P = cloud_P->location.OBJ;
  vector<vec4>& XYZI_P = cloud_P->registration.XYZI;
  vector<float>& Is_P = cloud_P->intensity.OBJ;
  vector<vec3>& key_P = cloud_P->registration.keypoints;
  int n = match_percentPts * XYZ_P.size()/100;

  //---> Initiate keypoints
  key_P.clear();
  for(int i=0; i<XYZ_P.size(); i=i+(int)XYZ_P.size()/(n)){
    key_P.push_back(XYZ_P[i]);
  }
  //---> If intensity initiate matrix
  if(Is_P.size() != 0){
    XYZI_P.clear();
    for(int i=0; i<XYZ_P.size(); i=i+(int)XYZ_P.size()/(n)){
      XYZI_P.push_back(vec4(XYZ_P[i], Is_P[i]));
    }
  }

  //Target data
  vector<vec3>& XYZ_Q = cloud_Q->location.OBJ;
  vector<vec4>& XYZI_Q = cloud_Q->registration.XYZI;
  vector<float>& Is_Q = cloud_Q->intensity.OBJ;

  if(Is_Q.size() != 0 && XYZI_Q.size() != XYZ_Q.size()){
    XYZI_Q.clear();
    for(int i=0; i<XYZ_Q.size(); i++){
      XYZI_Q.push_back(vec4(XYZ_Q[i], Is_Q[i]));
    }
  }

  //---------------------------*/
}
void ICP::compute_adaptativeGain(){
  //---------------------------

  if(SSE_new > SSE_old){
    gain = SSE_new/SSE_old;

    for(int i=0; i<3; i++){
      Xt[i] = Xt[i] * gain;
      Xr[i] = Xr[i] * gain;
    }
  }

  //---------------------------
}
void ICP::compute_checknPnD(){
  //---------------------------

  if(nD > 3) nD = 3;
  if(nP > 6) nP = 6;
  if(nP == 5 ) nP = 3;
  if(nP == 4 ) nP = 6;
  if(nP < nD) nP = nD;

  //---------------------------
}
