#include "RadarEquation.h"

#include "../../../../Operation/Function/Plotting.h"


//Constructor / Destructor
RadarEquation::RadarEquation(){
  //---------------------------

  plotManager = new Plotting();

  this->D = (3.8/1000); //FARO Focus 3D
  this->lambda = (905/1000000000); //FARO Focus 3D
  this->Pe = 0.02; //FARO Focus 3D

  //---------------------------
}
RadarEquation::~RadarEquation(){}

void RadarEquation::compute_RadarEquation(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  int size = Is.size();
  Ic.clear();
  //---------------------------

  //Make intensity correction according to radar's equation
  for(int i=0; i<size; i++){
    float Icorrect = Is[i] * (4 / (Pe * pow(D,2))) * (pow(dist[i],2) / pow(R_ref,2)) * (1 / pow(cosIt[i],2));
    Ic.push_back(Icorrect);
  }

  //---------------------------
  this->plot_intensityCorrected(cloud);
}
void RadarEquation::compute_IR2bycosIt(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  int size = Is.size();
  Ic.clear();
  //---------------------------

  //Make intensity correction according to radar's equation
  for(int i=0; i<size; i++){
    float Icorrect = Is[i] * (pow(dist[i],2) / pow(R_ref,2)) * (1 / pow(cosIt[i],2));
    Ic.push_back(Icorrect);
  }

  //---------------------------
  this->plot_intensityCorrected(cloud);
}
void RadarEquation::compute_IbyR2(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;

  float R_ref = 6.0f;
  int size = Is.size();
  Ic.clear();
  //---------------------------

  // Ic = Is * (R / Rs)
  for(int i=0; i<size; i++){
    float Icorr = Is[i] * (pow(dist[i],2) / pow(R_ref,2));
    Ic.push_back(Icorr);
  }

  //---------------------------
  this->plot_intensityCorrected(cloud);
}
void RadarEquation::compute_IbyCosIt(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& cosIt = subset->cosIt;
  int size = Is.size();
  Ic.clear();
  //---------------------------

  // Ic = Is / cos(It)
  for(int i=0; i<size; i++){
    float Icorr = Is[i] * (1 / pow(cosIt[i],2));
    Ic.push_back(Icorr);
  }

  //---------------------------
  this->plot_intensityCorrected(cloud);
}

//Plotting functions
void RadarEquation::plot_intensityCorrected(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  //---------------------------

  plotManager->plot_2histograms(Is, Ic);

  //---------------------------
}
