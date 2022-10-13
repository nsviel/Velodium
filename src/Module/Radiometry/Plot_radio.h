#ifndef PLOT_RADIO_H
#define PLOT_RADIO_H

#include "../../common.h"

class RadarEquation;
class Surfacic_simplified;
class Surfacic_global_piecewise;
class Surfacic_local;
class Surfacic_segmented;
class Separation_global;
class Separation_local;

class Engine_node;
class Scene;
class Plotting;
class Reference;
class Ref_Operation;
class Linearization;
class Attribut;
class BundleByClass;


class Plot_radio
{
public:
  //Constructor / Destructor
  Plot_radio(Engine_node* node);
  ~Plot_radio();

public:
  void compute_IRmeans(list<Cloud*>* list);
  void compute_IsItconcat(list<Cloud*>* list);
  
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
  void plot_IbyIt_cloud(Cloud* cloud);
  void plot_IbyR_cloud(Cloud* cloud);
  void plot_IbyR_data(Cloud* cloud);
  void plot_nDbycosIt();
  void plot_JbycosIt();
  void plot_SpectralonAllMeans();
  void plot_ParameterSpace();
  void plot_IbyItbyR();

private:
  //Classes
  Scene* sceneManager;
  Attribut* attribManager;
  Plotting* plotManager;
  BundleByClass* bundler;
  Reference* refManager;
  Ref_Operation* refopeManager;
  Linearization* linManager;

  RadarEquation* radio_radar;
  Surfacic_simplified* radio_surf_simpl;
  Surfacic_global_piecewise* radio_surf_glob;
  Surfacic_local* radio_surf_local;
  Surfacic_segmented* radio_surf_seg;
  Separation_global* radio_sepa_glob;
  Separation_local* radio_sepa_local;

  //Attributs
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
