#if !defined(LINEARIZATION_H)
#define LINEARIZATION_H

#include "../../../../common.h"

class Scene;
class Attribut;
class Reference;
class Ref_Operation;


class Linearization
{
public:
  //Constructor / Destructor
  Linearization(Scene* scene, Ref_Operation* ope);
  ~Linearization();

public:
  //General functions
  void algo_linearization(Collection* collection, int method);
  void algo_reverse(Collection* collection, int method);

  //Linearization
  void lin_SurfacicGlobal(Collection* collection);
  void lin_SeparationGlobal(Collection* collection);
  void lin_SurfacicLocal(Collection* collection);

  //Reverse
  void rev_SurfacicGlobal(Collection* collection);
  void rev_SeparationGlobal(Collection* collection);

  //Math functions
  void compute_SpectralonValues();
  void compute_gammaCoefficients();

  //Setters / Getters
  inline void set_mode(int value){this->mode = value;}

private:
  Scene* sceneManager;
  Ref_Operation* opeManager;
  Reference* refManager;

  float gamma_C, gamma_G, gamma_D;
  int mode;
};

#endif
