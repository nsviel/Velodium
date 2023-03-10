#if !defined(RADAR_EQUATION_H)
#define RADAR_EQUATION_H

#include "../../../../common.h"

class Plotting;


class RadarEquation
{
public:
  //Constructor / Destructor
  RadarEquation();
  ~RadarEquation();

public:
  //Method functions
  void compute_RadarEquation(Collection* collection);
  void compute_IR2bycosIt(Collection* collection);
  void compute_IbyR2(Collection* collection);
  void compute_IbyCosIt(Collection* collection);

  //Plotting
  void plot_intensityCorrected(Collection* collection);

  //Setters / Getters
  inline vector<float> get_Ic(){return Ic;}

private:
  Plotting* plotManager;

  vector<float> Ic;
  float D;
  float lambda;
  float Pe;
  float R_ref;
};

#endif
