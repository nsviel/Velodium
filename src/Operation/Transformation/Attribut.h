#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include "../../common.h"

#include <Eigen/Dense>

using namespace Eigen;

class Operation_node;
class Scene;
class Fitting;


class Attribut
{
public:
  //Constructor / Destructor
  Attribut(Operation_node* node_ope);
  ~Attribut();

public:
  //General
  void compute_cloudAttributs_all();
  void compute_cloudAttributs_list(list<Cloud*>* list);
  void compute_cloudAttributs(Subset* subset);
  void compute_distToScanner(Subset* subset);
  void compute_Distances(Subset* subset);
  void compute_cosIt(Subset* subset);

  void compute_subset_cosIt(Subset* subset);
  void compute_subset_distance(Subset* subset);

  void make_supressPoints(Subset* subset, vector<int>& idx);
  void make_supressPoint(Subset* subset, int id);
  void make_supressPoints(vector<vec3>& XYZ, vector<int>& idx);
  void cloudsData();
  vector<float> get_z_vector(vector<vec3>& xyz);

  //Color
  void set_cloud_color(Cloud* cloud, vec4 RGBA);
  void set_subset_color(Subset* subset, vec4 RGBA);
  void set_enhancedColor(Subset* subset);
  void set_randomizeAllPointsColor(Subset* subset);
  void set_restoreInitialColor(Subset* subset);
  void set_colorRGB_all();
  void set_colorRGB(Subset* subset);
  void set_colorI_all();
  void set_colorI(Subset* subset);

  //Normal
  void compute_normals(Subset* subset);
  void compute_normals_Hough(Subset* subset);
  void compute_normals_sphere(Subset* subset);
  void compute_normals_invert();
  void compute_normals_reorientToOrigin(Subset* subset);
  void compute_normals_planXaxis(Subset* subset);
  void compute_normals_planYaxis(Subset* subset);
  void compute_normals_planZaxis(Subset* subset);
  void compute_normals_planFitting(Subset* subset);
  void compute_checkForNan(Subset* subset);

  //Intensity
  void compute_intensityInversion();
  void compute_colorToIntensity(Subset* subset);
  void fct_convert255to2048(Subset* subset);
  void fct_convert2048to255(Subset* subset);
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
