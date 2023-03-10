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
  void compute_RadarEquation(Collection* cloud);
  void compute_IR2bycosIt(Collection* cloud);
  void compute_IbyR2(Collection* cloud);
  void compute_IbyCosIt(Collection* cloud);

  //Plotting
  void plot_intensityCorrected(Collection* cloud);

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
