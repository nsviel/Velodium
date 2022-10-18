#include "Surfacic_targeted.h"

#include "../Target/Ref_Operation.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/SpeudoInverse.h"


//Constructor / Destructor
Surfacic_targeted::Surfacic_targeted(Ref_Operation* opeClass){
  this->refopeManager = opeClass;
  //-------------------------------

  bundler = new BundleByClass();
  plotManager = new Plotting();

  this->verbose = false;
  this->Segment_1 = 5.7;
  this->Segment_2 = 14.2;
  this->m = 2;
  this->n = 2;

  //-------------------------------
}
Surfacic_targeted::~Surfacic_targeted(){}

//Main function
bool Surfacic_targeted::algo(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  Ic.clear(); Im.clear();
  //-------------------------------

  //Prepare references
  refopeManager->compute_Sphere_IbyR();
  vector<float> IbyR_R = refopeManager->get_Sphere_IbyR_R();

  //Check number of parameters
  this->algo_nbP();

  //Correection for all points
  for(int i=0; i<Is.size(); i++){
    vec2 infsup = algo_searchSegment(IbyR_R, dist[i]);
    this->algo_fitting(infsup);
    this->algo_correction(dist[i], cosIt[i], Is[i]);
  }

  this->repacking();

  //-------------------------------
  return true;
}

//Subfunctions
void Surfacic_targeted::algo_nbP(){
  nbP = 0;
  //-------------------------------

  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      nbP++;
    }
  }

  //-------------------------------
}
vec2 Surfacic_targeted::algo_searchSegment(vector<float> IbyR_R, float R){
  float sup = 100, inf = -100;
  float inf_out = -1, sup_out = -1;
  vec2 infsup;
  //-------------------------------

  //Search for inferior and superior
  for(int i=0; i<IbyR_R.size(); i++){
    float value = IbyR_R[i] - R;

    if(value < 0){
      if(value > inf){
        inf = value;
        inf_out = i;
      }
    }else{
      if(value < sup){
        sup = value;
        sup_out = i;
      }
    }
  }

  //Chack for limits
  if(inf_out == -1){
    inf_out = 0;
    sup_out = 1;
  }
  if(sup_out == -1){
    inf_out = IbyR_R.size()-1;
    sup_out = IbyR_R.size();
  }

  infsup = vec2(inf_out, sup_out);

  //-------------------------------
  return infsup;
}
void Surfacic_targeted::algo_fitting(vec2 infsup){
  //-------------------------------

  //vec3(R, cosIt, Is)
  vector<vec3> sphere_inf = refopeManager->get_sphereData(infsup.x);
  vector<vec3> sphere_sup = refopeManager->get_sphereData(infsup.y);

  vector<float> R_all, cIt_all, Is_all;
  for(int i=0; i<sphere_inf.size(); i++){
    R_all.push_back(sphere_inf[i].x);
    cIt_all.push_back(sphere_inf[i].y);
    Is_all.push_back(sphere_inf[i].z);
  }
  for(int i=0; i<sphere_sup.size(); i++){
    R_all.push_back(sphere_sup[i].x);
    cIt_all.push_back(sphere_sup[i].y);
    Is_all.push_back(sphere_sup[i].z);
  }

  MatrixXf J = this->compute_Jacobian(R_all, cIt_all);

  P = this->compute_SurfaceRegression(J, R_all, cIt_all, Is_all);

  //---------------------------
}
void Surfacic_targeted::algo_correction(float R, float cIt, float Is){
  //---------------------------

  //Check for NaN and deleteted point
  if(isnan(cIt) == true){
    cout<<"Surface targeting: There is some NaN with cosIt"<<endl;
  }

  float fit = multivariatePoly(P, cIt, R);
  Im.push_back(fit);

  float corr = Is * 0.87 / fit;
  Ic.push_back(corr);

  //---------------------------
}
void Surfacic_targeted::repacking(){
  //---------------------------

  for(int i=0; i<Im.size(); i++){
    if(Ic[i] < 0) Ic[i] = 0;
    if(Ic[i] > 1) Ic[i] = 1;
  }

  //---------------------------
}

//Surface fitting
float Surfacic_targeted::multivariatePoly(VectorXf P, float x, float y){
  int cpt = 0;
  float fit = 0;
  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      fit += P(cpt)*pow(x,i)*pow(y,j);
      cpt++;
    }
  }

  return fit;
}
MatrixXf Surfacic_targeted::compute_Jacobian(vector<float> R, vector<float> cIt){
  MatrixXf J = MatrixXf::Zero(R.size(), nbP);
  //--------------------------

  for(int k=0; k<R.size(); k++){
    float y = R[k];
    float x = cIt[k];

    int cpt = 0;
    for(int i=0; i<=m; i++){
      for(int j=0; j<=n; j++){
        J(k, cpt) = pow(x,i)*pow(y,j);
        cpt++;
      }
    }
  }

  //---------------------------
  return J;
}
VectorXf Surfacic_targeted::compute_SurfaceRegression(MatrixXf J, vector<float> R, vector<float> cIt, vector<float> Is){
  VectorXf E = VectorXf::Zero(R.size());
  VectorXf P = VectorXf::Zero(nbP);
  SpeudoInverse invManager;
  int iter = 10;
  //---------------------------

  for(int i=0; i<iter; i++){
    for(int j=0; j<R.size(); j++){
      float y = R[j];
      float x = cIt[j];
      float z = Is[j];

      //Fitting
      float fit = multivariatePoly(P, x, y);

      //Error vectors
      E(j) = fit - z;
    }

    //Optimization
    MatrixXf A, B, C, J_t;
    J_t = J.transpose();
    A = (J_t * J);
    B = (J_t * E);
    C = invManager.SpeudoInverse_orthoDecomp(A);

    if(A.determinant() == 0){
      cout<<"Problem inversion matrix !"<<endl;
    }

    P = P - C*B;
  }

  //---------------------------
  return P;
}

//Plotting
void Surfacic_targeted::plotting(){
  /*vector<vec3> seg;
  vector<float> fit;
  //-------------------------------

  for(int i=0; i<Is_all.size(); i++){
    float y = R_all[i];
    float x = cIt_all[i];
    float z = Is_all[i];

    seg.push_back(vec3(x,y,z));

    float ftting = multivariatePoly(P, x, y);
    fit.push_back(ftting);
  }

  plotManager->set_namePlot("3Dmap");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("cos({/Symbol a})");
  plotManager->set_Zlabel("Intensity [0;1]");
  plotManager->set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' title 'Raw data'");
  plotManager->set_Format_data2("with pm3d title 'Regression'");
  plotManager->plot_3DmapRegression(R_all, cIt_all, Is_all, fit);*/

  //-------------------------------
}
