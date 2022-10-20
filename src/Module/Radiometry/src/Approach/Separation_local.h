#if !defined(LOCAL_SEPARATION_H)
#define LOCAL_SEPARATION_H

#include "../../../../common.h"

class Plotting;
class Ref_Operation;
class BundleByClass;


class Separation_local
{
public:
  //Constructor / Destructor
  Separation_local(Ref_Operation* opeClass);
  ~Separation_local();

public:
  //Method function
  bool algo(Cloud* cloud);

  //Sub-functions
  bool algo_checking(Cloud* cloud);
  bool algo_ParameterSpace();
  bool algo_interpolation(Cloud* cloud);
  bool algo_correction(Cloud* cloud);

  //Math functions
  MatrixXf kNN_KdTreeFLANN(vector<vec2> PT_PS, vector<vec2> PT_query, bool normalized);
  MatrixXf compute_SurfaceRegression(vector<vec3>& PS, MatrixXf list_kNN);
  MatrixXf compute_Jacobian(vector<vec3>& PS, VectorXf list_kNN);
  float multivariatePoly(VectorXf P, float x, float y);
  float compute_nbP();

  //Plotting
  void plot_ParameterSpace();
  void plot_normalizedRA();
  void plot_RA();
  void plot_SurfaceRegression(VectorXf P, vector<vec3>& PS, VectorXf kNN, string t1);

  //Setters / Getters
  inline vector<float> get_Im(){return Im;}
  inline vector<float> get_Ic(){return Ic;}

  inline void set_interp_display(bool value){this->plotting = value;}
  inline void set_R_order(int value){this->n = value;}
  inline void set_It_order(int value){this->m = value;}
  inline void set_k_kNN(int value){this->knn = value;}
  inline void set_iterations(int value){this->iter = value;}
  inline void set_lambda(float value){this->lambda = value;}

private:
  BundleByClass* bundler;
  Plotting* plotManager;
  Ref_Operation* refopeManager;

  vector<vec3> PS_Sphere, PS_Spectralon;
  vector<float> Im, Ic;
  float lambda, precision_angle;
  int PS_size;
  int nbP, m, n, knn, iter;
  bool plotting;
};

#endif
