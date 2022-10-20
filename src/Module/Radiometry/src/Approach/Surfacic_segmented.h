#if !defined(SURFACIC_SEGMENTED_H)
#define SURFACIC_SEGMENTED_H

#include "../../../../common.h"

class Ref_Operation;


class Surfacic_segmented
{
public:
  //Constructor / Destructor
  Surfacic_segmented(Ref_Operation* opeClass);
  ~Surfacic_segmented();

public:
  //Method function
  bool algo(Cloud* cloud);

  //Sub-functions
  void algo_nbP();
  MatrixXf algo_ParameterSpace_segmented();
  vec2 algo_searchSegment(float R);
  void algo_correction(Cloud* cloud, MatrixXf list_P);
  void repacking();

  //Plotting
  void plot_oneSegment(vector<float>& dist, vector<float>& cosIt, vector<float>& Is, VectorXf P);
  void plot_SurfaceFitting();
  void plot_writeOnFile(MatrixXf list_P);

  //Surface fitting
  float multivariatePoly(VectorXf P, float x, float y);
  MatrixXf compute_Jacobian(vector<float> R, vector<float> cIt);
  VectorXf compute_SurfaceRegression(MatrixXf J, vector<float> R, vector<float> cIt, vector<float> I);

  //Setters / Getters
  inline void set_m(int value){this->m = value;}
  inline void set_n(int value){this->n = value;}
  inline vector<float> get_Im(){return Im;}
  inline vector<float> get_Ic(){return Ic;}

private:
  Ref_Operation* refopeManager;

  vector<float> Im, Ic;
  VectorXf P;
  int m, n, nbP;
  int iter;
};

#endif
