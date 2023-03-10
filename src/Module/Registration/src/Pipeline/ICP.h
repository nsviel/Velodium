#ifndef ICP_H
#define ICP_H

#include "ICP_Optimization.h"
#include "../../../../common.h"

class Transformation;
class ICP_Matching;
class ICP_Rejection;
//class Keypoint;
class Glyphs;
//class Algo_PCL;


class ICP
{
public:
  //Constructor / Destructor
  ICP(Glyphs* glyph);
  ~ICP();

public:
  //Main functions
  void init();
  void reset();
  void algo(Collection* cloud_P, Collection* cloud_Q);

  //Step functions
  void step_matching(Collection* cloud_P, Collection* cloud_Q);
  void step_rejection(Collection* cloud_P, Collection* cloud_Q);
  void step_optimization(Collection* cloud_P, Collection* cloud_Q);
  void step_transformation(Collection* cloud_P, Collection* cloud_Q);

  //Subfunctions
  float compute_SSE(Collection* cloud_P, Collection* cloud_Q);
  float compute_SSE_groundTruth(Collection* collection);
  float compute_MSE_groundTruth(Collection* collection);
  float compute_RMSE_groundTruth(Collection* collection);
  float compute_MAE_groundTruth(Collection* collection);

  void compute_prepareData(Collection* cloud_P, Collection* cloud_Q);
  void compute_checknPnD();
  void compute_adaptativeGain();

  //Assesseurs
  //inline vector<Uplet> get_idx(){return idx;}
  inline vec3 get_translat(){return Xt;}
  inline vec3 get_rotation(){return Xr;}
  inline float get_SSE(){return SSE;}
  inline float get_SSE_mean(){return SSE_mean;}
  inline float get_time_matching(){return time_match;}
  inline float get_time_rejection(){return time_reject;}
  inline float get_time_optimization(){return time_opti;}
  inline int get_nbParameters(){return nP;}
  inline int get_nbDimensions(){return nD;}
  //inline Keypoint* get_keyManager(){return keyManager;}
  inline int* get_matchingMethod(){return &m_match;}
  inline int* get_optimCOM(){return &m_com;}
  inline int* get_optimizationMethod(){return &m_optim;}
  inline bool* get_makeCorrespondences(){return &makeCorrespondences;}
  inline bool* get_rejectDistance(){return &reject_dist;}
  inline float* get_rejectDistance_threshold(){return &rejection_distance_threshold;}

  inline void set_matchingPercentPts(int value){this->match_percentPts = value;}
  inline void set_matchingNumberPts(int value){this->match_numberPts = value;}
  inline void set_rejectNormal(bool value){this->reject_normal = value;}
  inline void set_rejectDuplicata(bool value){this->reject_dupli = value;}
  inline void set_nbParameters(int value){this->nP = value; compute_checknPnD();}
  inline void set_nbDimensions(int value){this->nD = value; compute_checknPnD();}
  inline void set_dof_rotation(bool Rx, bool Ry, bool Rz){dof_Rx=Rx; dof_Ry=Ry; dof_Rz=Rz;}
  inline void set_dof_translation(bool tx, bool ty, bool tz){dof_tx=tx; dof_ty=ty; dof_tz=tz;}
  inline void set_dof_rot_weight(vec3 value){Xr_w = value;}
  inline void set_dof_tra_weight(vec3 value){Xt_w = value;}
  inline void set_icpGain(float value){optManager->set_lambda(value);}

private:
  ICP_Matching* matchManager;
  ICP_Optimization* optManager;
  ICP_Rejection* rejectManager;
  Transformation* transformManager;
  Glyphs* glyphManager;
  //Keypoint* keyManager;
  //Algo_PCL* pclManager;

  //Parameters
  vec3 Xt, Xr, Xt_w, Xr_w;
  float time_match, time_reject, time_opti;
  float SSE, SSE_old, SSE_new, SSE_mean;
  float gain;
  float rejection_distance_threshold;
  int m_match, m_rejec, m_optim, m_com;
  int nD, nP;
  int match_percentPts, match_numberPts;
  bool reject_normal, reject_dist, reject_dupli;
  bool dof_Rx, dof_Ry, dof_Rz;
  bool dof_tx, dof_ty, dof_tz;
  bool makeCorrespondences;
};

#endif
