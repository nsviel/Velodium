#include "ICP_Optimization.h"


//Constructor / Destructor
ICP_Optimization::ICP_Optimization(){
  //---------------------------

  this->lambda = 1;

  //---------------------------
}
ICP_Optimization::~ICP_Optimization(){}

//Algorithm
void ICP_Optimization::init(int XYZsize){
  //---------------------------

  this->size = XYZsize;
  this->Pt = vec3(0, 0, 0);
  this->Pr = vec3(0, 0, 0);

  //Check parameter size
  this->nP = 0;
  this->nP_t = 0;
  this->nP_r = 0;

  for(int i=0; i<3; i++){
    if(DOF[i] == true) nP_t++;
  }
  for(int i=3; i<6; i++){
    if(DOF[i] == true) nP_r++;
  }
  for(int i=0; i<6; i++){
    if(DOF[i] == true) nP++;
  }

  //---------------------------
}
void ICP_Optimization::algo_Newton(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM){
  /*vector<vec3>& XYZ_P_obj = cloud_P->registration.keypoints;
  vector<vec3>& XYZ_Q_obj = cloud_Q->registration.trgpoints;
  vector<float>& key_w = cloud_P_m->registration.keypoint_weight;
  //---------------------------

  this->init(XYZ_P_obj.size());

  //Compute Jacobian & Speudo-inverse
  MatrixXf J = compute_Jacobian(XYZ_P_obj, COM);
  MatrixXf Jinv = invManager.SpeudoInverse_QRDecomp(J);

  //Compute Errors
  VectorXf E = compute_Errors(XYZ_P_obj, XYZ_Q_obj, key_w);

  //Optimization
  VectorXf X = - lambda*(Jinv * E);

  //Paramater
  this->compute_param(X);

  //---------------------------*/
}
void ICP_Optimization::algo_Newton_separated(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM){
  /*int size = cloud_P->registration.keypoints.size();
  if(size == 0) return;
  //---------------------------

  this->init(size);
  this->algo_Xt(cloud_P, cloud_Q);
  this->algo_Xr(cloud_P, cloud_Q, COM);

  //---------------------------*/
}
void ICP_Optimization::algo_Xt(Cloud* cloud_P, Cloud* cloud_Q){
  /*vector<vec3>& XYZ_P = cloud_P->registration.keypoints;
  vector<vec3>& XYZ_Q = cloud_Q->registration.trgpoints;
  vector<float>& KEY_W = cloud_P->registration.keypoint_weight;
  if(nP_t == 0) return;
  //---------------------------

  //Compute Jacobian & Speudo-inverse
  MatrixXf J = compute_Jacobian_Xt();
  MatrixXf Jinv = invManager.SpeudoInverse_QRDecomp(J);

  //Compute Errors
  VectorXf E = compute_Errors(XYZ_P, XYZ_Q, KEY_W);

  //Optimization
  VectorXf Xt = - lambda*(Jinv * E);

  //Paramater
  this->compute_param_Pt(Xt);

  //---------------------------*/
}
void ICP_Optimization::algo_Xr(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM){
  /*vector<vec3>& XYZ_P = cloud_P->registration.keypoints;
  vector<vec3>& XYZ_Q = cloud_Q->registration.trgpoints;
  vector<float>& KEY_W = cloud_P->registration.keypoint_weight;
  if(nP_r == 0) return;

  MatrixXf J;
  MatrixXf Jinv;
  //---------------------------

  //Compute Errors
  VectorXf E = compute_Errors_Xr(XYZ_P, XYZ_Q, KEY_W);

  //Compute Jacobian & Speudo-inverse
  J = compute_Jacobian_Xr(XYZ_P, COM);
  Jinv = invManager.SpeudoInverse_QRDecomp(J);
  Vector3f Xr = - Jinv * E;

  //---------------------------
  this->compute_param_Pr(Xr);*/
}
void ICP_Optimization::algo_Xr_separated(Cloud* cloud_P, Cloud* cloud_Q, vec3 COM){
  /*vector<vec3>& XYZ_P = cloud_P->registration.keypoints;
  vector<vec3>& XYZ_Q = cloud_Q->registration.trgpoints;
  vector<float>& KEY_W = cloud_P->registration.keypoint_weight;
  if(nP_r == 0) return;

  MatrixXf J;
  MatrixXf Jinv;
  //---------------------------

  //Compute Errors
  VectorXf E = compute_Errors(XYZ_P, XYZ_Q, KEY_W);

  //Compute Jacobian & Speudo-inverse
  J = compute_Jacobian_Xr_x(XYZ_P, COM);
  Jinv = invManager.SpeudoInverse_QRDecomp(J);
  Vector3f Xr_x = - Jinv * E;

  //Compute Jacobian & Speudo-inverse
  J = compute_Jacobian_Xr_y(XYZ_P, COM);
  Jinv = invManager.SpeudoInverse_QRDecomp(J);
  Vector3f Xr_y = - Jinv * E;

  //Compute Jacobian & Speudo-inverse
  J = compute_Jacobian_Xr_z(XYZ_P, COM);
  Jinv = invManager.SpeudoInverse_QRDecomp(J);
  Vector3f Xr_z = - Jinv * E;

  //Vector of rotation parameters
  Vector3f Xr;
  Xr(0) = Xr_x(0);
  Xr(1) = Xr_y(0);
  Xr(2) = Xr_z(0);

  //---------------------------
  this->compute_param_Pr(Xr);*/
}

//Sub-functions
MatrixXf ICP_Optimization::compute_Jacobian(vector<vec3>& XYZ_P, vec3 COM){
  MatrixXf J = MatrixXf::Zero(size*6, nP);
  int cpt = 0;
  //---------------------------

  //Compute rotation matrices derivation
  Matrix3f dRx = compute_dRotx(0.0f);
  Matrix3f dRy = compute_dRoty(0.0f);
  Matrix3f dRz = compute_dRotz(0.0f);

  for(int i=0; i<size; i++){
    Vector3f XYZ (XYZ_P[i].x - COM.x, XYZ_P[i].y - COM.y, XYZ_P[i].z - COM.z);

    //Compute dR * point
    Vector3f dRxp = dRx * XYZ;
    Vector3f dRyp = dRy * XYZ;
    Vector3f dRzp = dRz * XYZ;

    // X axis
    int col = 0;
    if(DOF[0]){ J(cpt,col) = 1; col++;}
    if(DOF[1]){ J(cpt,col) = 0; col++;}
    if(DOF[2]){ J(cpt,col) = 0; col++;}

    if(DOF[3]){ J(cpt,col) = dRxp(0); col++;}
    if(DOF[4]){ J(cpt,col) = dRyp(0); col++;}
    if(DOF[5]){ J(cpt,col) = dRzp(0); col++;}

    // Y axis
    col = 0;
    if(DOF[0]){ J(cpt+1,col) = 0; col++;}
    if(DOF[1]){ J(cpt+1,col) = 1; col++;}
    if(DOF[2]){ J(cpt+1,col) = 0; col++;}

    if(DOF[3]){ J(cpt+1,col) = dRxp(1); col++;}
    if(DOF[4]){ J(cpt+1,col) = dRyp(1); col++;}
    if(DOF[5]){ J(cpt+1,col) = dRzp(1); col++;}

    // Z axis
    col = 0;
    if(DOF[0]){ J(cpt+2,col) = 0; col++;}
    if(DOF[1]){ J(cpt+2,col) = 0; col++;}
    if(DOF[2]){ J(cpt+2,col) = 1; col++;}

    if(DOF[3]){ J(cpt+2,col) = dRxp(2); col++;}
    if(DOF[4]){ J(cpt+2,col) = dRyp(2); col++;}
    if(DOF[5]){ J(cpt+2,col) = dRzp(2); col++;}

    cpt += 6;
  }

  //---------------------------
  return J;
}
MatrixXf ICP_Optimization::compute_Jacobian_Xt(){
  MatrixXf J = MatrixXf::Zero(size*3, nP_t);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<size; i++){
    // X axis
    int col = 0;
    if(DOF[0]){ J(cpt,col) = 1; col++;}
    if(DOF[1]){ J(cpt,col) = 0; col++;}
    if(DOF[2]){ J(cpt,col) = 0; col++;}

    // Y axis
    col = 0;
    if(DOF[0]){ J(cpt+1,col) = 0; col++;}
    if(DOF[1]){ J(cpt+1,col) = 1; col++;}
    if(DOF[2]){ J(cpt+1,col) = 0; col++;}

    // Z axis
    col = 0;
    if(DOF[0]){ J(cpt+2,col) = 0; col++;}
    if(DOF[1]){ J(cpt+2,col) = 0; col++;}
    if(DOF[2]){ J(cpt+2,col) = 1; col++;}

    cpt += 3;
  }

  //---------------------------
  return J;
}
MatrixXf ICP_Optimization::compute_Jacobian_Xr(vector<vec3>& XYZ_P, vec3 COM){
  MatrixXf J = MatrixXf::Zero(size*3, nP_r);
  int cpt = 0;
  //---------------------------

  //Compute rotation matrices derivation
  Matrix3f dRx = compute_dRotx(0.0f);
  Matrix3f dRy = compute_dRoty(0.0f);
  Matrix3f dRz = compute_dRotz(0.0f);

  //Compute Jacobian
  for(int i=0; i<size; i++){
    Vector3f XYZ (XYZ_P[i].x - COM.x, XYZ_P[i].y - COM.y, XYZ_P[i].z - COM.z);

    //Compute dR * point
    Vector3f dRxp = dRx * XYZ;
    Vector3f dRyp = dRy * XYZ;
    Vector3f dRzp = dRz * XYZ;

    // X axis
    int col = 0;
    if(DOF[3]){ J(cpt,col) = dRxp(0); col++;}
    if(DOF[4]){ J(cpt,col) = dRyp(0); col++;}
    if(DOF[5]){ J(cpt,col) = dRzp(0); col++;}

    // Y axis
    col = 0;
    if(DOF[3]){ J(cpt+1,col) = dRxp(1); col++;}
    if(DOF[4]){ J(cpt+1,col) = dRyp(1); col++;}
    if(DOF[5]){ J(cpt+1,col) = dRzp(1); col++;}

    // Z axis
    col = 0;
    if(DOF[3]){ J(cpt+2,col) = dRxp(2); col++;}
    if(DOF[4]){ J(cpt+2,col) = dRyp(2); col++;}
    if(DOF[5]){ J(cpt+2,col) = dRzp(2); col++;}

    cpt += 3;
  }

  //---------------------------
  return J;
}
MatrixXf ICP_Optimization::compute_Jacobian_Xr_x(vector<vec3>& XYZ_P, vec3 COM){
  Matrix3f dRx = compute_dRotx(0.0f);
  MatrixXf J = MatrixXf::Zero(size*3, 1);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<size; i++){
    Vector3f XYZ (XYZ_P[i].x - COM.x, XYZ_P[i].y - COM.y, XYZ_P[i].z - COM.z);

    //Compute dR * point
    Vector3f dRxp = dRx * XYZ;

    // X axis
    int col = 0;
    if(DOF[3]){ J(cpt,0) = dRxp(0); col++;}
    if(DOF[3]){ J(cpt+1,0) = dRxp(1); col++;}
    if(DOF[3]){ J(cpt+2,0) = dRxp(2); col++;}

    cpt += 3;
  }

  //---------------------------
  return J;
}
MatrixXf ICP_Optimization::compute_Jacobian_Xr_y(vector<vec3>& XYZ_P, vec3 COM){
  Matrix3f dRy = compute_dRoty(0.0f);
  MatrixXf J = MatrixXf::Zero(size*3, 1);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<size; i++){
    Vector3f XYZ (XYZ_P[i].x - COM.x, XYZ_P[i].y - COM.y, XYZ_P[i].z - COM.z);

    //Compute dR * point
    Vector3f dRyp = dRy * XYZ;

    // X axis
    int col = 0;
    if(DOF[3]){ J(cpt,0) = dRyp(0); col++;}
    if(DOF[3]){ J(cpt+1,0) = dRyp(1); col++;}
    if(DOF[3]){ J(cpt+2,0) = dRyp(2); col++;}

    cpt += 3;
  }

  //---------------------------
  return J;
}
MatrixXf ICP_Optimization::compute_Jacobian_Xr_z(vector<vec3>& XYZ_P, vec3 COM){
  Matrix3f dRz = compute_dRotz(0.0f);
  MatrixXf J = MatrixXf::Zero(size*3, 1);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<size; i++){
    Vector3f XYZ (XYZ_P[i].x - COM.x, XYZ_P[i].y - COM.y, XYZ_P[i].z - COM.z);

    //Compute dR * point
    Vector3f dRzp = dRz * XYZ;

    // X axis
    int col = 0;
    if(DOF[3]){ J(cpt,0) = dRzp(0); col++;}
    if(DOF[3]){ J(cpt+1,0) = dRzp(1); col++;}
    if(DOF[3]){ J(cpt+2,0) = dRzp(2); col++;}

    cpt += 3;
  }

  //---------------------------
  return J;
}

//Rotation matrice derivations
Matrix3f ICP_Optimization::compute_dRotx(float theta){
  Matrix3f rot = Matrix3f::Identity();
  //---------------------------

  rot(1,1) = -sin(theta);
  rot(1,2) = -cos(theta);
  rot(2,1) =  cos(theta);
  rot(2,2) = -sin(theta);

  //---------------------------
  return rot;
}
Matrix3f ICP_Optimization::compute_dRoty(float theta){
  Matrix3f rot = Matrix3f::Identity();
  //---------------------------

  rot(0,0) = -sin(theta);
  rot(2,0) = -cos(theta);
  rot(0,2) =  cos(theta);
  rot(2,2) = -sin(theta);

  //---------------------------
  return rot;
}
Matrix3f ICP_Optimization::compute_dRotz(float theta){
  Matrix3f rot = Matrix3f::Identity();
  //---------------------------

  rot(0,0) = -sin(theta);
  rot(0,1) = -cos(theta);
  rot(1,0) =  cos(theta);
  rot(1,1) = -sin(theta);

  //---------------------------
  return rot;
}

//Other sub-functions
VectorXf ICP_Optimization::compute_Errors(vector<vec3>& XYZ_P, vector<vec3>& XYZ_Q, vector<float>& key_w){
  VectorXf E = VectorXf::Zero(size * 3);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<XYZ_P.size(); i++){
    for(int j=0; j<3; j++){

      float P_dim = XYZ_P[i][j];
      float Q_dim = XYZ_Q[i][j];
      E(cpt) = key_w[i]*(P_dim - Q_dim);

      cpt++;
    }
  }

  //---------------------------
  return E;
}
VectorXf ICP_Optimization::compute_Errors_Xr(vector<vec3>& XYZ_P, vector<vec3>& XYZ_Q, vector<float>& key_w){
  VectorXf E = VectorXf::Zero(size * 3);
  int cpt = 0;
  //---------------------------

  for(int i=0; i<XYZ_P.size(); i++){
    for(int j=0; j<3; j++){

      float P_dim = XYZ_P[i][j] + Pt[j];
      float Q_dim = XYZ_Q[i][j];
      E(cpt) = key_w[i]*(P_dim - Q_dim);

      cpt++;
    }
  }

  //---------------------------
  return E;
}
void ICP_Optimization::compute_DOF(bool tx, bool ty, bool tz, bool Rx, bool Ry, bool Rz){
  DOF.clear();
  //---------------------------

  for(int i=0; i<6; i++){
    DOF.push_back(true);
  }

  if(!tx) DOF[0] = false;
  if(!ty) DOF[1] = false;
  if(!tz) DOF[2] = false;

  if(!Rx) DOF[3] = false;
  if(!Ry) DOF[4] = false;
  if(!Rz) DOF[5] = false;

  //---------------------------
}
void ICP_Optimization::compute_param(VectorXf X){
  Pt = vec3(0, 0, 0);
  Pr = vec3(0, 0, 0);
  int nb = 0;
  //---------------------------

  //Translation
  if(DOF[0]){ Pt[0] = X(nb); nb++;}
  if(DOF[1]){ Pt[1] = X(nb); nb++;}
  if(DOF[2]){ Pt[2] = X(nb); nb++;}

  //Rotation
  if(DOF[3]){ Pr[0] = X(nb); nb++;}
  if(DOF[4]){ Pr[1] = X(nb); nb++;}
  if(DOF[5]){ Pr[2] = X(nb); nb++;}

  //---------------------------
}
void ICP_Optimization::compute_param_Pt(VectorXf Xt){
  Pt = vec3(0, 0, 0);
  int nb = 0;
  //---------------------------

  //Translation
  if(DOF[0]){ Pt[0] = Xt(nb); nb++;}
  if(DOF[1]){ Pt[1] = Xt(nb); nb++;}
  if(DOF[2]){ Pt[2] = Xt(nb); nb++;}

  //---------------------------
}
void ICP_Optimization::compute_param_Pr(Vector3f Xr){
  Pr = vec3(0, 0, 0);
  int nb = 0;
  //---------------------------

  //Rotation
  if(DOF[3]){ Pr[0] = Xr(nb); nb++;}
  if(DOF[4]){ Pr[1] = Xr(nb); nb++;}
  if(DOF[5]){ Pr[2] = Xr(nb); nb++;}

  //---------------------------
}
