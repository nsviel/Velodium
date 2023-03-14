#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include "../../common.h"

#include <Eigen/Dense>

using namespace Eigen;

class Scene;
class Fitting;
class GPU_data;


class Attribut
{
public:
  //Constructor / Destructor
  Attribut();
  ~Attribut();

public:
  //General
  void compute_attribut_all();
  void compute_attribut_list(list<Collection*>* list);
  void compute_attribut_cloud(Collection* collection);
  void compute_attribut_subset(Cloud* cloud);
  void compute_distToScanner(Cloud* cloud);
  void compute_Distances(Cloud* cloud);
  void compute_cosIt(Cloud* cloud);

  void compute_subset_cosIt(Cloud* cloud);
  void compute_subset_distance(Cloud* cloud);

  void make_supressPoints(Cloud* cloud, vector<int>& idx);
  void make_supressPoints(Object_* object, vector<int>& idx);
  void make_supressPoint(Cloud* cloud, int id);
  void make_supressPoints(vector<vec3>& XYZ, vector<int>& idx);
  void cloudsData();
  vector<float> get_z_vector(vector<vec3>& xyz);

  //Normal
  void compute_normals(Cloud* cloud);
  void compute_normals_Hough(Cloud* cloud);
  void compute_normals_sphere(Cloud* cloud);
  void compute_normals_invert();
  void compute_normals_reorientToOrigin(Cloud* cloud);
  void compute_normals_planXaxis(Cloud* cloud);
  void compute_normals_planYaxis(Cloud* cloud);
  void compute_normals_planZaxis(Cloud* cloud);
  void compute_normals_planFitting(Cloud* cloud);
  void compute_checkForNan(Cloud* cloud);

  //Intensity
  void compute_intensityInversion();
  void compute_colorToIntensity(Cloud* cloud);
  void compute_intensityToColor(Cloud* cloud);
  void fct_convert255to2048(Cloud* cloud);
  void fct_convert2048to255(Cloud* cloud);
  void fct_moins();
  void fct_IsRange(vec2 range);
  vec2 get_IsRange();

  inline float get_sphereRadius(){return sphereRadius;}
  inline void set_sphereRadius(float value){this->sphereRadius = value;}

private:
  Scene* sceneManager;
  Fitting* fitManager;
  GPU_data* gpuManager;

  float sphereRadius;
};

#endif
