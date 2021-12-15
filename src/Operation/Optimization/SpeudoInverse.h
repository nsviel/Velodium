#ifndef SpeudoInverse_H
#define SpeudoInverse_H

#include "../../common.h"

#include <Eigen/Dense>

using namespace Eigen;


class SpeudoInverse
{
public:
  SpeudoInverse();
  ~SpeudoInverse();

public:
  MatrixXf SpeudoInverse_orthoDecomp(MatrixXf J);
  MatrixXf SpeudoInverse_LUDecomp(MatrixXf J);
  MatrixXf SpeudoInverse_QRDecomp(MatrixXf J);

private:
};

#endif
