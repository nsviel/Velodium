#include "SVD.h"

//Constructor / Destructor
SVD::SVD(){}
SVD::~SVD(){}

void SVD::algo_SVD(vector<vec3>& XYZ_icp, vector<vec3>& XYZ_trg){
/*  vec3 COM_icp = cloud_icp->location.COM;
  vec3 COM_trg = cloud_icp->location.COM;
  Vector3f com_icp(COM_icp[0], COM_icp[1], COM_icp[2]);
  Vector3f com_trg(COM_trg[0], COM_trg[1], COM_trg[2]);

  for(int i=idx.size()-1; i>=0; i--){
    Uplet tuple = idx[i];
    XYZ_trg[i] = XYZ_trg[tuple.idx2];
  }

  //Soustract Center Of Mass
  for(int i=0; i<XYZ_src.size(); i++){
    for(int j=0; j<3; j++){
      XYZ_src[i][j] = XYZ_src[i][j] - COM_icp[j];
    }
  }

  for(int i=0; i<XYZ_trg.size(); i++){
    for(int j=0; j<3; j++){
      XYZ_trg[i][j] = XYZ_trg[i][j] - COM_trg[j];
    }
  }

  //Convert to Eigen to computation
  MatrixXf P = glm_to_eigen(XYZ_src);
  MatrixXf Q = glm_to_eigen(XYZ_trg);

  //Matrix W
  MatrixXf W = P.transpose() * Q;

  //Compute SVD
  JacobiSVD<MatrixXf> svd(W, ComputeThinU | ComputeThinV);
  MatrixXf U = svd.matrixU();
  MatrixXf V = svd.matrixV();

  MatrixXf R;
  Vector3f t;
  if(svd.rank() == 3) //The optimal solution is unique
  {
    R = U * V;
    t = com_icp - R *com_trg;
  }
  else
    say("Too bad");

  //Extract parameters
  deltaX(0) = atan2(R(2,1), R(2,2));
  deltaX(1) = atan2(-R(2,0), sqrt(pow(R(2,1),2) + pow(R(2,2),2)));
  deltaX(2) = atan2(R(1,0), R(0,0));
  deltaX(3) = t(0);
  deltaX(4) = t(1);
  deltaX(5) = t(2);
  extractParameters();*/
}
void SVD::extractParameters(){

}
