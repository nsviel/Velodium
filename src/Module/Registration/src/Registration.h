#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "../../../common.h"

class Scene;
class Glyphs;
class Transformation;
class ICP;
class Attribut;
class Plotting;
class Node_engine;


class Registration
{
public:
  //Constructor / Destructor
  Registration(Node_engine* node);
  ~Registration();

public:
  //Main functions
  void make_Iteration();

  //Subfunctions
  void reset();
  void restart();
  void print_ICP();
  void colorization(Collection* cloud_P, Collection* cloud_Q);

  //Accesseurs
  inline ICP* get_icpManager(){return icpManager;}
  inline vector<float> get_vec_SSE(){return vec_SSE;}
  inline vector<int> get_vec_iter(){return vec_iter;}
  inline vector<vec3> get_vec_trans(){return vec_trans;}
  inline vector<vec3> get_vec_rotat(){return vec_rotat;}
  inline float get_duration(){return duration;}
  inline int get_iter(){return nbIter;}
  inline int* get_nbIter_max(){return &nbIter_max;}
  inline int* get_colorMethode(){return &colorMeth;}
  inline float* get_SSE_max(){return &SSE_max;}

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Glyphs* glyphManager;
  ICP* icpManager;
  Plotting* plotManager;

  vector<float> vec_SSE;
  vector<int> vec_iter;
  vector<vec3> vec_trans, vec_rotat;
  float duration;
  float SSE, SSE_max, SSE_mean;
  int nbIter, nbIter_max;
  int colorMeth;
};

#endif
