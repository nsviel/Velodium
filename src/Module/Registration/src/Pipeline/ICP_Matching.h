#ifndef ICP_MATCHING_H
#define ICP_MATCHING_H

#include "../../struct_misc.h"
#include "../../../../common.h"


class ICP_Matching
{
public:
  //Constructor / Destructor
  ICP_Matching();
  ~ICP_Matching();

public:
  //Simple matching
  void algo_directMatching(Cloud* subset_P, Cloud* subset_Q);
  void algo_userSelection(Cloud* subset_P, Cloud* subset_Q);

  //Geometric based
  void algo_NN_BruteForce_ICCP(Cloud* subset_P, Cloud* subset_Q);
  void algo_NN_BruteForce(Cloud* subset_P, Cloud* subset_Q);
  void algo_NN_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q);

  //Intensity based
  void algo_NI_BruteForce(Cloud* subset_P, Cloud* subset_Q);
  void algo_NI_NN(Cloud* subset_P, Cloud* subset_Q);
  void algo_NI_NN_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q);
  void algo_NI_KdTreeFLANN(Cloud* subset_P, Cloud* subset_Q);
  void algo_NI_NN_KdTreeNanoFLANN(Cloud* subset_P, Cloud* subset_Q);

  inline vector<Uplet> get_idx(){return idx;}

  void algo_PFH(Cloud* subset_P, Cloud* subset_Q);

private:
  Collection* cloud_P;
  Collection* cloud_Q;
  vector<Uplet> idx;
};

#endif
