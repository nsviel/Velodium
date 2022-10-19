#ifndef MI_H
#define MI_H

#include "../../../../common.h"


class MutualInformation
{
public:
  //Constructor / Destructor
  MutualInformation();
  ~MutualInformation();

public:
  bool algo(Subset* subset_P, Subset* subset_Q);
  float entropy(vector<float>& Is);
  float mutualEntropy(vector<float>& I1, vector<float>& I2);
  vector<float> hist(vector<float>& Is);

private:

};

#endif
