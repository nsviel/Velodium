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
  void compute_RadarEquation(Cloud* cloud);
  void compute_IR2bycosIt(Cloud* cloud);
  void compute_IbyR2(Cloud* cloud);
  void compute_IbyCosIt(Cloud* cloud);

  //Plotting
  void plot_intensityCorrected(Cloud* cloud);

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
