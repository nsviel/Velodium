#include "SpeudoInverse.h"


//Constructor / Destructor
SpeudoInverse::SpeudoInverse(){}
SpeudoInverse::~SpeudoInverse(){}

MatrixXf SpeudoInverse::SpeudoInverse_orthoDecomp(MatrixXf J){
  //---------------------------

  MatrixXf Ji = J.completeOrthogonalDecomposition().pseudoInverse();

  if(Ji.determinant() == 0){
    cout<<"Problem inversion matrix !"<<endl;
  }

  //---------------------------
  return Ji;
}
MatrixXf SpeudoInverse::SpeudoInverse_QRDecomp(MatrixXf J){
  MatrixXf A, C, Ji;
  //---------------------------

  A = (J.transpose() * J);
  Eigen::ColPivHouseholderQR<MatrixXf> qr(A);
  C = qr.inverse();
  Ji = C*J.transpose();

  if(!qr.isInvertible()){
    cout<<"Problem inversion matrix !"<<endl;
  }

  //---------------------------
  return Ji;
}
MatrixXf SpeudoInverse::SpeudoInverse_LUDecomp(MatrixXf J){
  MatrixXf A, C, Ji;
  //---------------------------

  A = (J.transpose() * J);
  Eigen::FullPivLU<MatrixXf> lu(A);
  C = lu.inverse();
  Ji = C*J.transpose();

  if(!lu.isInvertible()){
    cout<<"Problem inversion matrix !"<<endl;
  }

  //---------------------------
  return Ji;
}
