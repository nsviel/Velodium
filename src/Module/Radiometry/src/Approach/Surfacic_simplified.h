#if !defined(METHOD_SIMPLIFIED_SURFACIC_H)
#define METHOD_SIMPLIFIED_SURFACIC_H

#include "../../../../common.h"

class Plotting;
class PolyRegression;
class BundleByClass;
class Reference;


class Surfacic_simplified
{
public:
  //Constructor / Destructor
  Surfacic_simplified(Reference* refClass);
  ~Surfacic_simplified();

public:
  //Method functions
  bool algo_Bretagne(Cloud* cloud);

  //Sub-functions
  bool compute_linearRegression();
  bool compute_quadraRegression();
  bool compute_correction(Cloud* cloud);
  bool compute_IIR2(vector<float>& Ib, vector<float>& Ib_dist);

  //Plotting
  void plot_bundleByClass(Cloud* cloud);
  void plot_linearRegression(Cloud* cloud);
  void plot_quadraticRegression(Cloud* cloud);
  void plot_intensityCorrection(Cloud* cloud);

  //Setters / Getters
  inline vector<float> get_Im(){return Im;}
  inline vector<float> get_Ic(){return Ic;}

private:
  BundleByClass* bundler;
  Plotting* plotManager;
  Reference* refManager;

  vector<float> lin_A, lin_B, mdist;
  vector<float> quad_A, quad_B;
  vector<float> Im, Ic;
  int nA, nB;
};

#endif
