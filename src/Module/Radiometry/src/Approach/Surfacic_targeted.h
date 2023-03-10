#if !defined(SURFACIC_TARGETED_H)
#define SURFACIC_TARGETED_H

#include "../../../../common.h"

class Plotting;
class BundleByClass;
class Ref_Operation;


class Surfacic_targeted
{
public:
  //Constructor / Destructor
  Surfacic_targeted(Ref_Operation* opeClass);
  ~Surfacic_targeted();

public:
  //Method function
  bool algo(Collection* collection);

  //Sub-functions
  void algo_nbP();
  vec2 algo_searchSegment(vector<float> IbyR_R, float R);
  void algo_fitting(vec2 infsup);
  void algo_correction(float R, float cIt, float Is);
  void repacking();
  void plotting();

  //Surface fitting
  float multivariatePoly(VectorXf P, float x, float y);
  MatrixXf compute_Jacobian(vector<float> R, vector<float> cIt);
  VectorXf compute_SurfaceRegression(MatrixXf J, vector<float> R, vector<float> cIt, vector<float> I);

  //Specific functions
  bool compute_ReferencePoints(VectorXf P_b, VectorXf P_m, VectorXf P_e);
  void compute_normalizeCoeffs(VectorXf P);
  void compute_error(Collection* collection);


  //Setters / Getters
  inline void set_m(int value){this->m = value;}
  inline void set_n(int value){this->n = value;}
  inline vector<float> get_Im(){return Im;}
  inline vector<float> get_Ic(){return Ic;}
  inline float* get_segment_1(){return &Segment_1;}
  inline float* get_segment_2(){return &Segment_2;}

private:
  Ref_Operation* refopeManager;
  BundleByClass* bundler;
  Plotting* plotManager;

  int m, n, nbP;
  bool verbose;
  float Segment_1, Segment_2;
  float lambda_b, lambda_m, lambda_e;

  vector<float> Im, Ic;
  vector<float> I01_tot, R_tot, cIt_tot;
  vector<float> I2048_beg, R_beg, cIt_beg;
  vector<float> I2048_mid, R_mid, cIt_mid;
  vector<float> I2048_end, R_end, cIt_end;

  VectorXf P;
  VectorXf P_b, P_m, P_e;
};

#endif
