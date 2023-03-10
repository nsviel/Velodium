#include "Surfacic_simplified.h"

#include "../Target/Reference.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/Polyfit.h"
#include "../../../../Specific/color.h"
#include "../../../../Specific/Function/fct_math.h"


//Constructor / Destructor
Surfacic_simplified::Surfacic_simplified(Reference* refClass){
  this->refManager = refClass;
  //---------------------------

  bundler = new BundleByClass();
  plotManager = new Plotting();

  this->nA = 5;
  this->nB = 10;

  //---------------------------
}
Surfacic_simplified::~Surfacic_simplified(){}

//Method function
bool Surfacic_simplified::algo_Bretagne(Collection* collection){
  //---------------------------

  this->compute_linearRegression();
  this->compute_quadraRegression();
  this->compute_correction(cloud);

  //---------------------------
  return true;
}

//Sub-functions
bool Surfacic_simplified::compute_linearRegression(){
  list<Collection*>* list = refManager->get_listSphere();
  lin_A.clear();
  lin_B.clear();
  mdist.clear();
  //---------------------------

  //Add a point at 0
  lin_A.push_back(0);
  lin_B.push_back(0);
  mdist.push_back(0);

  for(int i=0; i<list->size(); i++){
    Collection* collection = *next(list->begin(),i);

    //Bundle by classes
    bundler->compute_bundleByClass_maxAngle(cloud, 5, 70);
    vector<float> Ib = bundler->get_Ib();
    vector<float> Ib_dist = bundler->get_Ib_dist();
    vector<float> Ib_cosIt = bundler->get_Ib_cosIt();

    //Compute IIR2
    this->compute_IIR2(Ib, Ib_dist);

    //Compute polynomial fitting
    vector<float> lin_coeffs = polyfit(Ib_cosIt, Ib, 1);

    //recover A & B coeffs
    lin_A.push_back(lin_coeffs[1]);
    lin_B.push_back(lin_coeffs[0]);
    mdist.push_back(fct_mean(Ib_dist));

    //Coefficient of determination R²
    float R2 = fct_R2(Ib_cosIt, Ib, lin_coeffs);
  }

  //---------------------------
  if(lin_A.size() == 0){
    cout<<"--> Problem computing linear regression"<<endl;
    return false;
  }
  return true;
}
bool Surfacic_simplified::compute_quadraRegression(){
  //---------------------------

  //Coeffs A - Compute polynomial fitting
  quad_A = polyfit(mdist, lin_A, nA);

  //Coeffs B - Compute polynomial fitting
  quad_B = polyfit(mdist, lin_B, nB);

  //Coefficient of determination R²
  float R2_A = fct_R2(mdist, lin_A, quad_A);
  float R2_B = fct_R2(mdist, lin_B, quad_B);

  //---------------------------
  return true;
}
bool Surfacic_simplified::compute_correction(Collection* collection){
  Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), 0);
  vector<float>& Is = cloud->I;
  vector<float>& dist = cloud->R;
  vector<float>& cosIt = cloud->cosIt;
  Im.clear(); Ic.clear();
  float Imin = 0;
  //---------------------------

  //Compute estimated values
  vector<float> curv_A = polyval(dist, quad_A, quad_A.size()-1);
  vector<float> curv_B = polyval(dist, quad_B, quad_B.size()-1);

  //Intensity correction
  for(int i=0; i<Is.size(); i++){
    //I model [0,2048]
    float Im_2048 = (curv_A[i]/pow(dist[i],2)) * cosIt[i] + curv_B[i]/pow(dist[i],2) - Imin;

    //I model [0,1]
    float mod = Im_2048 / 2048;
    Im.push_back(mod);

    //I corrected
    float corr = 0.87 * Is[i] / mod;
    Ic.push_back(corr);
  }

  //---------------------------
  return true;
}
bool Surfacic_simplified::compute_IIR2(vector<float>& Ib, vector<float>& Ib_dist){
  float Imin = 0;
  //---------------------------

  //Conversion in [0,2048] scale
  for(int i=0; i<Ib.size(); i++){
    Ib[i] = Ib[i] * 2048;
  }

  //Compute (I-Imin)R²
  for(int i=0; i<Ib.size(); i++){
    Ib[i] = (Ib[i] - Imin) * pow(Ib_dist[i], 2);
  }

  //---------------------------
  return true;
}

//Plotting
void Surfacic_simplified::plot_bundleByClass(Collection* collection){
  //---------------------------

  bundler->compute_bundleByClass(cloud, 5);
  bundler->plot_intensityBundle(cloud);

  //---------------------------
}
void Surfacic_simplified::plot_linearRegression(Collection* collection){
  //---------------------------

  //Bundle by classes
  bundler->compute_bundleByClass_maxAngle(cloud, 5, 70);
  vector<float> Ib = bundler->get_Ib();
  vector<float> Ib_dist = bundler->get_Ib_dist();
  vector<float> Ib_cosIt = bundler->get_Ib_cosIt();
  vector<float> Ib_std = bundler->get_Ib_std();

  //Compute IIR2
  this->compute_IIR2(Ib, Ib_dist);

  //Compute polynomial fitting
  vector<float> coeffs = polyfit(Ib_cosIt, Ib, 1);

  //Linear regression line
  vector<float> curve = polyval(Ib_cosIt, coeffs, coeffs.size()-1);

  cout<<"A: "<<coeffs[1]<<" | B: "<<coeffs[0]<<endl;

  //Plotting
  plotManager->set_namePlot("Bretagne_linRegre");
  plotManager->set_Xlabel("cos({/Symbol a})");
  plotManager->set_Ylabel("(I_{raw}-I_{min})d^2");
  plotManager->plot_CurveErrorRegression(Ib_cosIt, Ib, curve, Ib_std);

  //---------------------------
}
void Surfacic_simplified::plot_quadraticRegression(Collection* collection){
  this->algo_Bretagne(cloud);
  //---------------------------

  //Linear regression line
  std::vector<float> curv_A, curv_B;
  curv_A = polyval(mdist, quad_A, quad_A.size()-1);
  curv_B = polyval(mdist, quad_B, quad_B.size()-1);

  //Plotting
  plotManager->set_namePlot("Bretagne_quadraRegre");
  plotManager->set_Xlabel("R");
  plotManager->set_Ylabel("Regression coefficients");
  vector<string> dataFormeA;
  string col = color_a1;
  dataFormeA.push_back("with points pt 7 ps 1 lc rgb '"+col+"' title 'A'");
  col = color_a3;
  dataFormeA.push_back("with points pt 13 ps 1 lc rgb '"+col+"' title 'B'");
  col = color_a2;
  dataFormeA.push_back("with lines lc rgb '"+col+"' title 'Regr A'");
  col = color_a4;
  dataFormeA.push_back("with lines lc rgb '"+col+"' title 'Regr B'");
  plotManager->set_dataFormA(dataFormeA);
  plotManager->plot_2Regression(mdist, lin_A, lin_B, curv_A, curv_B);

  //---------------------------
}
void Surfacic_simplified::plot_intensityCorrection(Collection* collection){
  Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), 0);
  vector<float>& Is = cloud->I;
  this->algo_Bretagne(cloud);
  //--------------------------

  //Check error
  if(Im.size() != Ic.size() || Im.size() != Is.size()){
    cout<<"--> Problem correction"<<endl;
  }

  //Plotting
  cout<<"-> Plotting linear relationship"<<endl;
  plotManager->plot_Curve_lin(Im, Is);

  cout<<"-> Plotting histograms"<<endl;
  plotManager->plot_2histograms(Is, Ic);

  //--------------------------
}
