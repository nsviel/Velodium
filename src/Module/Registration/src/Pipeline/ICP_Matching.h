#ifndef ICP_MATCHING_H
#define ICP_MATCHING_H

#include "../../../../Engine/Data/struct_misc.h"
#include "../../../../common.h"


class ICP_Matching
{
public:
  //Constructor / Destructor
  ICP_Matching();
  ~ICP_Matching();

public:
  //Simple matching
  void algo_directMatching(Subset* subset_P, Subset* subset_Q);
  void algo_userSelection(Subset* subset_P, Subset* subset_Q);

  //Geometric based
  void algo_NN_BruteForce_ICCP(Subset* subset_P, Subset* subset_Q);
  void algo_NN_BruteForce(Subset* subset_P, Subset* subset_Q);
  void algo_NN_KdTreeFLANN(Subset* subset_P, Subset* subset_Q);

  //Intensity based
  void algo_NI_BruteForce(Subset* subset_P, Subset* subset_Q);
  void algo_NI_NN(Subset* subset_P, Subset* subset_Q);
  void algo_NI_NN_KdTreeFLANN(Subset* subset_P, Subset* subset_Q);
  void algo_NI_KdTreeFLANN(Subset* subset_P, Subset* subset_Q);
  void algo_NI_NN_KdTreeNanoFLANN(Subset* subset_P, Subset* subset_Q);

  inline vector<Uplet> get_idx(){return idx;}

  void algo_PFH(Subset* subset_P, Subset* subset_Q);

private:
  Cloud* cloud_P;
  Cloud* cloud_Q;
  vector<Uplet> idx;
};

#endif
