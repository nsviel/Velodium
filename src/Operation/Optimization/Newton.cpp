#include "Newton.h"

//Constructor / Destructor
Newton::Newton(){
  this->nb_iter = 10;
  this->X = VectorXf::Zero(6);
}
Newton::~Newton(){}

//Main functions
void Newton::init(vector<vec3>& XYZ, vector<bool>& DOF){
  this->size = XYZ.size();

  //Check parameter size
  this->nP = 0;
  for(int i=0; i<DOF.size(); i++){
    if(DOF[i] == true) nP++;
  }

  //Check parameter initialization
  if(X.size() != nP){
    this->X = VectorXf::Zero(nP);
  }
}
vector<float> Newton::algo_Newton(Cloud* cloud_P, Cloud* cloud_Q, vector<bool>& DOF, vector<Uplet> idx){
  Subset* subset_P = *next(cloud_P->subset.begin(), 0);
  Subset* subset_Q = *next(cloud_Q->subset.begin(), 0);
  Subset* subset_P_buf = *next(cloud_P->subset_buffer.begin(), 0);
  Subset* subset_Q_buf = *next(cloud_Q->subset_buffer.begin(), 0);
  //---------------------

  vector<vec3>& XYZ_P_obj = subset_P->xyz;
  vector<vec3>& XYZ_Q_obj = subset_Q->xyz;
  vector<vec3>& XYZ_P_buf = subset_P_buf->xyz;
  vector<vec3>& XYZ_Q_buf = subset_Q_buf->xyz;
  this->init(XYZ_P_buf, DOF);

  //Compute Jacobian & Speudo-inverse
  MatrixXf J = compute_Jacobian(XYZ_P_buf, DOF);
  MatrixXf Jinv = invManager.SpeudoInverse_LUDecomp(J);

  //Compute Errors
  VectorXf E = compute_Errors(XYZ_P_obj, XYZ_Q_obj, idx);

  //Optimization
  X = X - lambda*(Jinv * E);

  //Retrieve translation and rotation parameters
  vector<float> X_out = extractParameters(DOF);

  //---------------------
  return X_out;
}
vector<float> Newton::extractParameters(vector<bool>& DOF){
  vector<float> X_out = {0,0,0,0,0,0};
  //---------------------

  int nb = 0;
  if(DOF[0]){ X_out[0] = X(nb); nb++; }
  if(DOF[1]){ X_out[1] = X(nb); nb++; }
  if(DOF[2]){ X_out[2] = X(nb); nb++; }

  if(DOF[3]){ X_out[3] = X(nb); nb++; }
  if(DOF[4]){ X_out[4] = X(nb); nb++; }
  if(DOF[5]){ X_out[5] = X(nb); nb++; }

  //---------------------
  return X_out;
}

//Subfunctions
MatrixXf Newton::compute_Jacobian(vector<vec3>& XYZ_1, vector<bool>& DOF){
  Matrix3f dRx = compute_dRotx(X(3));
  Matrix3f dRy = compute_dRoty(X(4));
  Matrix3f dRz = compute_dRotz(X(5));
  MatrixXf J = MatrixXf::Zero(size*3, nP);
  //----------------------------

  int cpt = 0;
  for(int i=0; i<size; i++){
    Vector3f XYZ;
    XYZ(0) = XYZ_1[i].x;
    XYZ(1) = XYZ_1[i].y;
    XYZ(2) = XYZ_1[i].z;

    //Compute dR * pi
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

    cpt += 3;
  }

  //----------------------------
  return J;
}
VectorXf Newton::compute_Errors(vector<vec3>& XYZ_1, vector<vec3>& XYZ_2, vector<Uplet> idx){
  MatrixXf Error = MatrixXf::Zero(size, 3);
  Uplet tuple;
  //---------------------

  for(int i=0; i<idx.size(); i++){
    for(int j=0; j<3; j++){
      tuple = idx[i];

      float P = XYZ_1[tuple.idx1][j];
      float Q = XYZ_2[tuple.idx2][j];
      Error(i,j) = P - Q;
    }
  }
  VectorXf E = vectorization(Error);

  //---------------------
  return E;
}
VectorXf Newton::vectorization(MatrixXf mat){
  VectorXf vec = VectorXf::Zero(size * 3);

  int cpt = 0;
  for(int i=0; i<size; i++)
  {
    vec(cpt) = mat(i,0);
    vec(cpt+1) = mat(i,1);
    vec(cpt+2) = mat(i,2);

    cpt += 3;
  }

  return vec;
}

Matrix3f Newton::compute_Rotx(float theta){
  Matrix3f rot;
  rot(1,1) =  cos(theta);
  rot(1,2) = -sin(theta);
  rot(2,1) =  sin(theta);
  rot(2,2) =  cos(theta);

  return rot;
}
Matrix3f Newton::compute_Roty(float theta){
  Matrix3f rot;
  rot(0,0) =  cos(theta);
  rot(2,0) = -sin(theta);
  rot(0,2) =  sin(theta);
  rot(2,2) =  cos(theta);

  return rot;
}
Matrix3f Newton::compute_Rotz(float theta){
  Matrix3f rot;
  rot(0,0) =  cos(theta);
  rot(0,1) = -sin(theta);
  rot(1,0) =  sin(theta);
  rot(1,1) =  cos(theta);

  return rot;
}
Matrix3f Newton::compute_dRotx(float theta){
  Matrix3f rot = Matrix3f::Identity();
  rot(1,1) = -sin(theta);
  rot(1,2) = -cos(theta);
  rot(2,1) =  cos(theta);
  rot(2,2) = -sin(theta);

  return rot;
}
Matrix3f Newton::compute_dRoty(float theta){
  Matrix3f rot = Matrix3f::Identity();
  rot(0,0) = -sin(theta);
  rot(2,0) = -cos(theta);
  rot(0,2) =  cos(theta);
  rot(2,2) = -sin(theta);

  return rot;
}
Matrix3f Newton::compute_dRotz(float theta){
  Matrix3f rot = Matrix3f::Identity();
  rot(0,0) = -sin(theta);
  rot(0,1) = -cos(theta);
  rot(1,0) =  cos(theta);
  rot(1,1) = -sin(theta);

  return rot;
}
