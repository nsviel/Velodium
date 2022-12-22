#include "Separation_global.h"

#include "../Target/Reference.h"
#include "../Target/Ref_Operation.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/Polyfit.h"
#include "../../../../Specific/fct_math.h"
#include "../../../../Specific/color.h"


//Constructor / Destructor
Separation_global::Separation_global(Reference* refClass){
  this->refManager = refClass;
  //---------------------------

  bundler = new BundleByClass();
  plotManager = new Plotting();
  refopeManager = new Ref_Operation(refClass);

  this->R_n = 9;
  this->cIt_n = 3;
  this->matlab_coeffs = true;
  this->verbose = false;

  //---------------------------
}
Separation_global::~Separation_global(){}

//Method function
bool Separation_global::algo(Cloud* cloud){
  //---------------------------

  this->algo_angle();
  this->algo_distance();
  this->algo_correction(cloud);

  //---------------------------
  return true;
}

//Sub-functions
bool Separation_global::algo_angle(){
  list<Cloud*>* list_ref = refManager->get_list_99p_10m_xd();
  refopeManager->compute_Spectralon_IbycIt(10);
  //---------------------------

  //Data
  vector<float> Is_99 = refopeManager->get_Spec_IbycIt_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbycIt_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbycIt_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbycIt_I01_10();

  vector<float> cIt_99 = refopeManager->get_Spec_IbycIt_cIt_99();
  vector<float> cIt_50 = refopeManager->get_Spec_IbycIt_cIt_50();
  vector<float> cIt_25 = refopeManager->get_Spec_IbycIt_cIt_25();
  vector<float> cIt_10 = refopeManager->get_Spec_IbycIt_cIt_10();
  //------------------------------------

  //Check data
  if(Is_99.size() == 0 || Is_50.size() == 0 || Is_25.size() == 0 || Is_10.size() == 0){
    cout<<"Problem Spectralon intensity"<<endl;
    cout<<Is_99.size()<<endl;
    cout<<Is_50.size()<<endl;
    cout<<Is_25.size()<<endl;
    cout<<Is_10.size()<<endl;
  }
  if(cIt_99.size() == 0 || cIt_50.size() == 0 || cIt_25.size() == 0 || cIt_10.size() == 0){
    cout<<"Problem Spectralon angle"<<endl;
    cout<<cIt_99.size()<<endl;
    cout<<cIt_50.size()<<endl;
    cout<<cIt_25.size()<<endl;
    cout<<cIt_10.size()<<endl;
  }
  for(int i=0; i<Is_99.size(); i++){
    Is_99[i] = Is_99[i]*2048;
    Is_50[i] = Is_50[i]*2048;
    Is_25[i] = Is_25[i]*2048;
    Is_10[i] = Is_10[i]*2048;
  }

  //Coefficients
  coeff_99 = polyfit(cIt_99, Is_99, cIt_n);
  coeff_50 = polyfit(cIt_50, Is_50, cIt_n);
  coeff_25 = polyfit(cIt_25, Is_25, cIt_n);
  coeff_10 = polyfit(cIt_10, Is_10, cIt_n);

  //Functions
  this->compute_MeanError_angle();
  this->compute_normalizeCoeffs_angle();

  //---------------------------
  return true;
}
bool Separation_global::algo_distance(){
  refopeManager->compute_Spectralon_IbyR();

  vector<float> Is_99 = refopeManager->get_Spec_IbyR_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbyR_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbyR_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbyR_I01_10();

  vector<float> R_99 = refopeManager->get_Spec_IbyR_R_99();
  vector<float> R_50 = refopeManager->get_Spec_IbyR_R_50();
  vector<float> R_25 = refopeManager->get_Spec_IbyR_R_25();
  vector<float> R_10 = refopeManager->get_Spec_IbyR_R_10();
  //---------------------------

  //Check data
  if(Is_99.size() == 0 || Is_50.size() == 0 || Is_25.size() == 0 || Is_10.size() == 0){
    cout<<"Problem Spectralon intensity"<<endl;
    cout<<Is_99.size()<<endl;
    cout<<Is_50.size()<<endl;
    cout<<Is_25.size()<<endl;
    cout<<Is_10.size()<<endl;
  }
  if(R_99.size() == 0 || R_50.size() == 0 || R_25.size() == 0 || R_10.size() == 0){
    cout<<"Problem Spectralon angle"<<endl;
    cout<<R_99.size()<<endl;
    cout<<R_50.size()<<endl;
    cout<<R_25.size()<<endl;
    cout<<R_10.size()<<endl;
  }
  for(int i=0; i<Is_99.size(); i++){
    Is_99[i] = Is_99[i]*2048;
    Is_50[i] = Is_50[i]*2048;
    Is_25[i] = Is_25[i]*2048;
    Is_10[i] = Is_10[i]*2048;
  }

  //Get I by R coefficients
  coeff_99 = polyfit(R_99, Is_99, R_n);
  coeff_50 = polyfit(R_50, Is_50, R_n);
  coeff_25 = polyfit(R_25, Is_25, R_n);
  coeff_10 = polyfit(R_10, Is_10, R_n);

  //Coefficient normalization
  this->compute_normalizeCoeffs_distance();
  this->compute_MeanError_distance();

  //---------------------------
  if(matlab_coeffs){
    R_coeffs.clear();
    R_coeffs = {
      2187.57086099582,-663.14006028681,259.451057283256,-46.4034892074906,4.49053401831041,-0.256992603296084,0.0089797412296937,-0.00018846506993685,2.18478574759541e-06,-1.07585833400427e-08
    };
  }
  return true;
}
bool Separation_global::algo_correction(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cIt = subset->cosIt;
  Im.clear(); Ic.clear();
  //---------------------------

  //Regression curve
  R_curve = polyval(dist, R_coeffs, R_n);
  cIt_curve = polyval(cIt, cIt_coeffs, cIt_n);

  //Reference coefficient
  float M2 = R_lambda[0] * cIt_lambda[0];
  M2 = 0.755*2048 * 0.755*2048;

  //Correction
  for(int i=0; i<Is.size(); i++){
    // I model
    float mod =  R_curve[i] * cIt_curve[i];
    Im.push_back(mod);

    // I corrected
    float corr = (Is[i]*2048) * M2 / mod;
    Ic.push_back(corr);
  }

  //---------------------------
  for(int i=0; i<Im.size(); i++){
    //Prevent from over or under correction
    if(Ic[i] > 2048 ){
      Ic[i] = 2048;
    }else if(Ic[i] < 0){
      Ic[i] = 0;
    }

    Im[i] = (Im[i])/2048;
    Ic[i] = (Ic[i])/2048;
  }
  if(Is.size() != Im.size() || Is.size() != Ic.size()){
    cout<<"Error with Tan method with Ic size"<<endl;
    return false;
  }else{
    return true;
  }
}

//Math functions
void Separation_global::compute_normalizeCoeffs_angle(){
  vector<float> coeff_99b, coeff_50b, coeff_25b, coeff_10b;
  int last = coeff_99.size()-1;
  //---------------------------

  for(int i=0; i<coeff_99.size(); i++){
    coeff_99b.push_back(coeff_99[i] / coeff_99[last]);
    coeff_50b.push_back(coeff_50[i] / coeff_50[last]);
    coeff_25b.push_back(coeff_25[i] / coeff_25[last]);
    coeff_10b.push_back(coeff_10[i] / coeff_10[last]);
  }

  cIt_coeffs.clear();
  cIt_coeffs_normalized.clear();

  for(int i=0; i<coeff_99b.size(); i++){
    float coeff = coeff_99b[i] + coeff_50b[i] + coeff_25b[i] + coeff_10b[i];
    coeff = coeff/4;
    cIt_coeffs_normalized.push_back(coeff);

    float coeff_raw = coeff_99[i] + coeff_50[i] + coeff_25[i] + coeff_10[i];
    coeff_raw = coeff_raw/4;
    cIt_coeffs.push_back(coeff_raw);
  }

  //---------------------------
}
void Separation_global::compute_normalizeCoeffs_distance(){
  vector<float> coeff_99b, coeff_50b, coeff_25b, coeff_10b;
  //---------------------------

  int last = coeff_99.size()-1;
  for(int i=0; i<coeff_99.size(); i++){
    coeff_99b.push_back(coeff_99[i] / coeff_99[last]);
    coeff_50b.push_back(coeff_50[i] / coeff_50[last]);
    coeff_25b.push_back(coeff_25[i] / coeff_25[last]);
    coeff_10b.push_back(coeff_10[i] / coeff_10[last]);
  }

  R_coeffs.clear();
  R_coeffs_normalized.clear();

  for(int i=0; i<coeff_99b.size(); i++){
    float coeff = coeff_99b[i] + coeff_50b[i] + coeff_25b[i] + coeff_10b[i];
    coeff = coeff/4;
    R_coeffs_normalized.push_back(coeff);

    float coeff_raw = coeff_99[i] + coeff_50[i] + coeff_25[i] + coeff_10[i];
    coeff_raw = coeff_raw/4;
    R_coeffs.push_back(coeff_raw);
  }

  //---------------------------
}
void Separation_global::compute_MeanError_angle(){
  vector<float> Is_99 = refopeManager->get_Spec_IbycIt_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbycIt_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbycIt_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbycIt_I01_10();

  vector<float> cIt_99 = refopeManager->get_Spec_IbycIt_cIt_99();
  vector<float> cIt_50 = refopeManager->get_Spec_IbycIt_cIt_50();
  vector<float> cIt_25 = refopeManager->get_Spec_IbycIt_cIt_25();
  vector<float> cIt_10 = refopeManager->get_Spec_IbycIt_cIt_10();

  ebyn_cIt_n.clear();
  ebyn_cIt_e.clear();
  //---------------------------

  for(int i=1; i<11; i++){
    vector<float> coeff_99 = polyfit(cIt_99, Is_99, i);
    vector<float> coeff_50 = polyfit(cIt_50, Is_50, i);
    vector<float> coeff_25 = polyfit(cIt_25, Is_25, i);
    vector<float> coeff_10 = polyfit(cIt_10, Is_10, i);

    vector<float> curve_99 = polyval(cIt_99, coeff_99, i);
    vector<float> curve_50 = polyval(cIt_50, coeff_50, i);
    vector<float> curve_25 = polyval(cIt_25, coeff_25, i);
    vector<float> curve_10 = polyval(cIt_10, coeff_10, i);

    // Error vector
    vector<float> E;
    for(int j=0; j<Is_99.size(); j++){
      float err_99 = abs(Is_99[j] - curve_99[j]);
      float err_50 = abs(Is_50[j] - curve_50[j]);
      float err_25 = abs(Is_25[j] - curve_25[j]);
      float err_10 = abs(Is_10[j] - curve_10[j]);

      float meanE = (err_99 + err_50 + err_25 + err_10)/4;

      E.push_back(meanE);
    }

    // Mean error
    float theta = fct_std(E) / sqrt(E.size() + 1);

    //cout<<"cIt: n = "<<i<<" -> e = "<<theta<<endl;

    ebyn_cIt_e.push_back(theta);
    ebyn_cIt_n.push_back(i);
  }

  //---------------------------
  if(ebyn_cIt_e.size() != ebyn_cIt_n.size()){
    cout<<"Size problem"<<endl;
  }
}
void Separation_global::compute_MeanError_distance(){
  vector<float> Is_99 = refopeManager->get_Spec_IbyR_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbyR_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbyR_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbyR_I01_10();

  vector<float> R_99 = refopeManager->get_Spec_IbyR_R_99();
  vector<float> R_50 = refopeManager->get_Spec_IbyR_R_50();
  vector<float> R_25 = refopeManager->get_Spec_IbyR_R_25();
  vector<float> R_10 = refopeManager->get_Spec_IbyR_R_10();

  ebyn_R_n.clear();
  ebyn_R_e.clear();
  //---------------------------

  for(int i=1; i<11; i++){
    vector<float> coeff_99 = polyfit(R_99, Is_99, i);
    vector<float> coeff_50 = polyfit(R_50, Is_50, i);
    vector<float> coeff_25 = polyfit(R_25, Is_25, i);
    vector<float> coeff_10 = polyfit(R_10, Is_10, i);

    vector<float> curve_99 = polyval(R_99, coeff_99, i);
    vector<float> curve_50 = polyval(R_50, coeff_50, i);
    vector<float> curve_25 = polyval(R_25, coeff_25, i);
    vector<float> curve_10 = polyval(R_10, coeff_10, i);

    // Error vector
    vector<float> E;
    for(int j=0; j<Is_99.size(); j++){
      float err_99 = abs(Is_99[j] - curve_99[j]);
      float err_50 = abs(Is_50[j] - curve_50[j]);
      float err_25 = abs(Is_25[j] - curve_25[j]);
      float err_10 = abs(Is_10[j] - curve_10[j]);

      float meanE = (err_99 + err_50 + err_25 + err_10)/4;

      E.push_back(meanE);
    }

    // Mean error
    float theta = fct_std(E) / sqrt(E.size() + 1);

    //cout<<"R: n = "<<i<<" -> e = "<<theta<<endl;

    ebyn_R_e.push_back(theta);
    ebyn_R_n.push_back(i);
  }

  //---------------------------
  if(ebyn_R_e.size() != ebyn_R_n.size()){
    cout<<"Size problem"<<endl;
  }
}
void Separation_global::compute_Reference_angle(){
  list<Cloud*>* list_ref = refManager->get_list_99p_10m_xd();
  string nameRef = "Spectralon_99p_10m_00d";
  //---------------------------

  for(int i=0; i<list_ref->size() ;i++){
    Cloud* cloud_25 = *next(list_ref->begin(),i);
    Subset* subset = *next(cloud_25->subset.begin(), 0);
    if(subset->name == nameRef){
      vector<float> d_ref{ fct_mean(subset->cosIt) };
      cIt_lambda = polyval(d_ref, cIt_coeffs, cIt_n);
    }
  }

  //---------------------------
  if(cIt_lambda.size() == 0){
    cout<<"Problem reference method Tan"<<endl;
  }
}
void Separation_global::compute_Reference_distance(){
  list<Cloud*>* list_ref = refManager->get_list_99p_xm();
  string nameRef = "Spectralon_99p_10m";
  R_lambda.clear();
  //---------------------------

  for(int i=0; i<list_ref->size() ;i++){
    Cloud* cloud_ref = *next(list_ref->begin(),i);
    Subset* subset = *next(cloud_ref->subset.begin(), 0);
    if(subset->name == nameRef){
      vector<float> R_ref{ fct_mean(subset->R) };
      R_lambda = polyval(R_ref, R_coeffs, R_n);
    }
  }

  //---------------------------
}

//Plotting functions
void Separation_global::plot_IbyR(){
  this->algo_distance();
  //---------------------------

  vector<float> R_99 = refopeManager->get_Spec_IbyR_R_99();
  vector<float> R_50 = refopeManager->get_Spec_IbyR_R_50();
  vector<float> R_25 = refopeManager->get_Spec_IbyR_R_25();
  vector<float> R_10 = refopeManager->get_Spec_IbyR_R_10();

  vector<float> Is_99 = refopeManager->get_Spec_IbyR_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbyR_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbyR_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbyR_I01_10();

  for(int i=0; i<Is_99.size(); i++){
    Is_99[i] = Is_99[i] * 2048;
    Is_50[i] = Is_50[i] * 2048;
    Is_25[i] = Is_25[i] * 2048;
    Is_10[i] = Is_10[i] * 2048;
  }
  vector<float> R_curve = polyval(R_99, R_coeffs, R_n);

  plotManager->Clear();
  plotManager->set_namePlot("IbyR_regression_algoTan");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("Intensité I_{raw}");
  vector<string> dataFormA;
  dataFormA.push_back("with linespoints ls 1 pt 7 ps 0.5 lc rgb '" color_a1 "' title '99%'");
  dataFormA.push_back("with linespoints ls 1 pt 5 ps 0.5 lc rgb '" color_a2 "' title '57%'");
  dataFormA.push_back("with linespoints ls 1 pt 9 ps 0.5 lc rgb '" color_a3 "' title '28%'");
  dataFormA.push_back("with linespoints ls 1 pt 13 ps 0.5 lc rgb '" color_a4 "' title '11%'");
  plotManager->set_dataFormA(dataFormA);
  plotManager->set_Format_data2("smooth uniq with lines lw 0.5 lc rgb 'red' title 'I_{cal}'");
  plotManager->plot_MultipleCurveRegression(R_99, Is_99, Is_50, Is_25, Is_10, R_curve);

  //---------------------------
}
void Separation_global::plot_IbyR_atomic(){
  vector<float> Is_99 = refopeManager->get_Spec_IbyR_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbyR_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbyR_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbyR_I01_10();

  vector<float> R_99 = refopeManager->get_Spec_IbyR_R_99();
  vector<float> R_50 = refopeManager->get_Spec_IbyR_R_50();
  vector<float> R_25 = refopeManager->get_Spec_IbyR_R_25();
  vector<float> R_10 = refopeManager->get_Spec_IbyR_R_10();

  if(verbose){
    vector<float> wei;
    wei.push_back(1);
    for(int i=1; i<R_99.size()-1; i++){
      wei.push_back(0.5);
    }
    wei.push_back(1);
  }

  vector<float> coeff_99 = polyfit_boost(R_99, Is_99, R_n);
  vector<float> coeff_50 = polyfit_boost(R_50, Is_50, R_n);
  vector<float> coeff_25 = polyfit_boost(R_25, Is_25, R_n);
  vector<float> coeff_10 = polyfit_boost(R_10, Is_10, R_n);

  vector<float> curve_99 = polyval(R_99, coeff_99, R_n);
  vector<float> curve_50 = polyval(R_50, coeff_50, R_n);
  vector<float> curve_25 = polyval(R_25, coeff_25, R_n);
  vector<float> curve_10 = polyval(R_10, coeff_10, R_n);
  //---------------------------

  //----------------------------
  plotManager->set_namePlot("IbyR_regression_algoTan_atomic");
  plotManager->set_Xlabel("Distance R");
  plotManager->set_Ylabel("Intensity I [0;2048]");
  vector<string> dataFormA;
  dataFormA.push_back("with linespoints ls 1 pt 7 ps 0.5 lc rgb '" color_a1 "' title '99%'");
  dataFormA.push_back("with linespoints ls 1 pt 5 ps 0.5 lc rgb '" color_a2 "' title '57%'");
  dataFormA.push_back("with linespoints ls 1 pt 9 ps 0.5 lc rgb '" color_a3 "' title '28%'");
  dataFormA.push_back("with linespoints ls 1 pt 13 ps 0.5 lc rgb '" color_a4 "' title '11%'");
  plotManager->set_dataFormA(dataFormA);
  plotManager->set_Format_data2("smooth uniq with lines lw 0.5 lc rgb 'red' notitle");
  plotManager->plot_MultipleCurveRegression_tot(R_99, Is_99, Is_50, Is_25, Is_10, curve_99, curve_50, curve_25, curve_10);
}
void Separation_global::plot_IbyIt(){
  this->algo_angle();
  refopeManager->compute_Spectralon_IbycIt(10);
  vector<float> It_99 = refopeManager->get_Spec_IbycIt_It_99();

  vector<float> cIt_99 = refopeManager->get_Spec_IbycIt_cIt_99();
  vector<float> cIt_50 = refopeManager->get_Spec_IbycIt_cIt_50();
  vector<float> cIt_25 = refopeManager->get_Spec_IbycIt_cIt_25();
  vector<float> cIt_10 = refopeManager->get_Spec_IbycIt_cIt_10();

  vector<float> Is_99 = refopeManager->get_Spec_IbycIt_I01_99();
  vector<float> Is_50 = refopeManager->get_Spec_IbycIt_I01_50();
  vector<float> Is_25 = refopeManager->get_Spec_IbycIt_I01_25();
  vector<float> Is_10 = refopeManager->get_Spec_IbycIt_I01_10();
  //---------------------------

  for(int i=0; i<Is_99.size(); i++){
    Is_99[i] = Is_99[i] * 2048;
    Is_50[i] = Is_50[i] * 2048;
    Is_25[i] = Is_25[i] * 2048;
    Is_10[i] = Is_10[i] * 2048;
  }
  vector<float> cIt_curve = polyval(cIt_99, cIt_coeffs, cIt_n);

  if(verbose){
    say("-----");
    say("cos(It)");
    for(int i=0; i<cIt_99.size(); i++){
      cout<<cIt_99[i]<<" "<<cIt_50[i]<<" "<<cIt_25[i]<<" "<<cIt_10[i]<<endl;
    }
    say("-----");
    say("Is");
    for(int i=0; i<Is_99.size(); i++){
      cout<<Is_99[i]<<" "<<Is_50[i]<<" "<<Is_25[i]<<" "<<Is_10[i]<<endl;
    }

    //Verbose coefficients
    cout<< "Tan angle normalized coefficients: "<<endl;
    for(int i=0; i<cIt_coeffs_normalized.size(); i++){
      cout<<cIt_coeffs_normalized[i]<<endl;
    }
    cout<< "Tan angle coefficients: "<<endl;
    for(int i=0; i<cIt_coeffs_normalized.size(); i++){
      cout<<cIt_coeffs[i]<<endl;
    }
  }

  //---------------------------
  plotManager->Clear();
  plotManager->set_namePlot("IbycIt_regression_algoTan");
  plotManager->set_Xlabel("Angle d'incidence (°)");
  plotManager->set_Ylabel("Intensité I_{raw}");
  vector<string> dataFormA;
  dataFormA.push_back("with linespoints ls 1 pt 7 ps 0.5 lc rgb '" color_a1 "' title '99%'");
  dataFormA.push_back("with linespoints ls 1 pt 5 ps 0.5 lc rgb '" color_a2 "' title '57%'");
  dataFormA.push_back("with linespoints ls 1 pt 9 ps 0.5 lc rgb '" color_a3 "' title '28%'");
  dataFormA.push_back("with linespoints ls 1 pt 13 ps 0.5 lc rgb '" color_a4 "' title '11%'");
  plotManager->set_dataFormA(dataFormA);
  plotManager->set_Format_data2("smooth uniq with lines lw 0.5 lc rgb 'red' title 'I_{cal}'");
  plotManager->plot_MultipleCurveRegression(It_99, Is_99, Is_50, Is_25, Is_10, cIt_curve);
}
void Separation_global::plot_MeanError(){
  this->algo_angle();
  this->algo_distance();
  //---------------------------

  //---------------------------
  plotManager->set_namePlot("MeanError_algoTan");
  plotManager->set_Xlabel("N");
  plotManager->set_Ylabel("{/Symbol s}_{dist}");
  plotManager->set_Format_data1("with linespoints pt 7 ps 0.3 lw 1 lc rgb 'black' notitle");
  plotManager->plot_Curve(ebyn_R_n, ebyn_R_e);

  plotManager->set_namePlot("MeanError_algoTan");
  plotManager->set_Xlabel("N");
  plotManager->set_Ylabel("{/Symbol s}_{cos({/Symbol a})}");
  plotManager->set_Format_data1("with linespoints pt 7 ps 0.3 lw 1 lc rgb 'black' notitle");
  plotManager->plot_Curve(ebyn_cIt_n, ebyn_cIt_e);
}
void Separation_global::plot_PolyRegression(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cIt = subset->cosIt;
  this->algo(cloud);
  //---------------------------

  for(int i=0; i<Is.size(); i++){
    Is[i] = Is[i] * 2048;
  }

  cIt_curve = polyval(cIt, cIt_coeffs, cIt_n);
  R_curve = polyval(dist, R_coeffs, R_n);

  for(int i=0; i<cIt_curve.size(); i++){
    R_curve[i] = R_curve[i] * (1/R_lambda[0]);
    cIt_curve[i] = cIt_curve[i] * (1/cIt_lambda[0]);
  }

  //---------------------------
  plotManager->set_namePlot("IbyR_algoTan");
  plotManager->set_Xlabel("R");
  plotManager->set_Ylabel("I");
  plotManager->set_Format_data1("title 'I_{initial}'");
  plotManager->set_Format_data2("title 'Regression'");
  plotManager->plot_Regression(dist, Is, R_curve);

  plotManager->set_namePlot("IbycIt_algoTan");
  plotManager->set_Xlabel("cos({/Symbol a})");
  plotManager->set_Ylabel("I");
  plotManager->set_Format_data1("title 'I_{initial}'");
  plotManager->set_Format_data2("title 'Regression'");
  plotManager->plot_Regression(cIt, Is, cIt_curve);
}
void Separation_global::plot_bundleByClass(Cloud* cloud){
  //---------------------------

  bundler->compute_bundleByClass(cloud, 5);
  bundler->plot_intensityBundle(cloud);

  //---------------------------
}
void Separation_global::plot_Icorr(Cloud* cloud){
  this->algo(cloud);
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  //--------------------------------

  plotManager->plot_Curve_lin(Is, Im);
  plotManager->plot_2histograms(Is, Ic);

  //---------------------------
}
