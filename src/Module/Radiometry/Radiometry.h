#ifndef RADIOMETRY_H
#define RADIOMETRY_H

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


class Radiometry
{
public:
  //Constructor / Destructor
  Radiometry(Engine_node* node);
  ~Radiometry();

public:
  //Correction Methods
  void algo_RadarEquation(int num);
  void algo_surfacicSimplified(Cloud* cloud);
  void algo_separationGlobal(Cloud* cloud);
  void algo_surfacicGlobalPiecewise(Cloud* cloud);
  void algo_surfacicLocal(Cloud* cloud);
  void algo_surfacicSegmented(Cloud* cloud);

  //Correction functions
  void correction_allClouds();
  void correction_allClouds_Iini();
  void correction_allClouds_WtRefs();
  void correction_oneCloud(Cloud* cloud);
  void correction_oneCloud_Iini(Cloud* cloud);
  bool compute_RadioCorrection(Cloud* cloud);

  //Subfunctions
  void compute_sortByIt(vector<float>& vec1, vector<float>& vec2);
  void compute_SpectralonAjustement();
  bool compute_list();
  bool compute_subListSpectralon();
  bool check_cloudData(Cloud* cloud);
  void set_referenceON(bool value);
  void remove_References();
  void wrt_results();

  //Plotting functions
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

  //Setters / Getters
  inline int* get_algoCorrection(){ return &algoSelected;}
  inline Surfacic_global_piecewise* get_Surfacic_global_piecewise(){return radio_surf_glob;}
  inline Surfacic_local* get_Surfacic_local(){return radio_surf_local;}
  inline Surfacic_simplified* get_Surfacic_simplified(){return radio_surf_simpl;}
  inline Surfacic_segmented* get_Surfacic_segmented(){return radio_surf_seg;}
  inline Separation_global* get_Separation_global(){return radio_sepa_glob;}
  inline Linearization* get_linManager(){return linManager;}
  inline Reference* get_refManager(){return refManager;}

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
