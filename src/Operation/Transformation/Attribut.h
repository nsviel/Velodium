#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include "../../common.h"

#include <Eigen/Dense>

using namespace Eigen;

class Scene;
class Fitting;


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
  void compute_attribut_subset(Cloud* subset);
  void compute_distToScanner(Cloud* subset);
  void compute_Distances(Cloud* subset);
  void compute_cosIt(Cloud* subset);

  void compute_subset_cosIt(Cloud* subset);
  void compute_subset_distance(Cloud* subset);

  void make_supressPoints(Cloud* subset, vector<int>& idx);
  void make_supressPoint(Cloud* subset, int id);
  void make_supressPoints(vector<vec3>& XYZ, vector<int>& idx);
  void cloudsData();
  vector<float> get_z_vector(vector<vec3>& xyz);

  //Normal
  void compute_normals(Cloud* subset);
  void compute_normals_Hough(Cloud* subset);
  void compute_normals_sphere(Cloud* subset);
  void compute_normals_invert();
  void compute_normals_reorientToOrigin(Cloud* subset);
  void compute_normals_planXaxis(Cloud* subset);
  void compute_normals_planYaxis(Cloud* subset);
  void compute_normals_planZaxis(Cloud* subset);
  void compute_normals_planFitting(Cloud* subset);
  void compute_checkForNan(Cloud* subset);

  //Intensity
  void compute_intensityInversion();
  void compute_colorToIntensity(Cloud* subset);
  void fct_convert255to2048(Cloud* subset);
  void fct_convert2048to255(Cloud* subset);
  void fct_moins();
  void fct_IsRange(vec2 range);
  vec2 get_IsRange();

  inline float get_sphereRadius(){return sphereRadius;}
  inline void set_sphereRadius(float value){this->sphereRadius = value;}

private:
  Scene* sceneManager;
  Fitting* fitManager;

  float sphereRadius;
};

#endif
