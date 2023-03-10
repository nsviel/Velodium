#if !defined(PLOT_RADIO_H)
#define PLOT_RADIO_H

#include "../../../common.h"

class Node_engine;
class Scene;
class Plotting;
class Reference;
class Ref_Operation;
class Attribut;
class BundleByClass;


class Plot_radio
{
public:
  //Constructor / Destructor
  Plot_radio(Node_engine* node);
  ~Plot_radio();

public:
  void compute_IRmeans(list<Collection*>* list);
  void compute_IsItconcat(list<Collection*>* list);

  void plot_3Dmap();
  void plot_2Dmap();
  void plot_IbyR();
  void plot_IbyR_all();
  void plot_IbyR_Spectralon();
  void plot_IbyR_Sphere();
  void plot_IbyR_both();
  void plot_IbyCosIt(bool normalised);
  void plot_IbyCosIt_Sphere(bool normalised);
  void plot_IbyCosIt_all_Sphere();
  void plot_IbyIt_Spectralon(bool normalised);
  void plot_IbyCosIt_all();
  void plot_IbyIt_all_Sphere();
  void plot_IbyIt_allAngle_Spectralon(bool normalised);
  void plot_IbyIt_allPercentage_Spectralon_05m();
  void plot_IbyIt_allPercentage_Spectralon_10m();
  void plot_IbyIt_allPercentage_Spectralon_20m();
  void plot_IbyIt_allPercentage_Spectralon_30m();
  void plot_IbyIt_allPercentage_Spectralon_40m();
  void plot_IbyIt_Sphere(bool normalised);
  void plot_IbyIt_cloud(Collection* collection);
  void plot_IbyR_cloud(Collection* collection);
  void plot_IbyR_data(Collection* collection);
  void plot_nDbycosIt();
  void plot_JbycosIt();
  void plot_SpectralonAllMeans();
  void plot_ParameterSpace();
  void plot_IbyItbyR();

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Plotting* plotManager;
  BundleByClass* bundler;
  Reference* refManager;
  Ref_Operation* refopeManager;

  vector<vector<float>> I_saved, R_saved;
  vector<string> N_saved;
  vector<float> Is_std, Ic_std;
  vector<float> Is_CV, Ic_CV;
  vector<float> Im, Ic, Is;
  vector<float> R_mean, Is_mean, It_mean, std_mean;
  vector<float> Is_conc, It_conc;
  int algoSelected;
};

#endif
