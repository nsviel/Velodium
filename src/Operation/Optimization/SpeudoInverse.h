#ifndef SpeudoInverse_H
#define SpeudoInverse_H

#include "../../common.h"

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
