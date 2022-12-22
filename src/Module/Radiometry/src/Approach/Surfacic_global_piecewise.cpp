#include "Surfacic_global_piecewise.h"

#include "../Target/Ref_Operation.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Specific/fct_math.h"


//Constructor / Destructor
Surfacic_global_piecewise::Surfacic_global_piecewise(Ref_Operation* opeClass){
  this->refopeManager = opeClass;
  //-------------------------------

  this->Segment_1 = 5.7;
  this->Segment_2 = 14.2;
  this->m = 2;
  this->n = 2;

  //-------------------------------
}
Surfacic_global_piecewise::~Surfacic_global_piecewise(){}

//Method function
bool Surfacic_global_piecewise::algo(Cloud* cloud){
  //-------------------------------

  this->algo_nbP();
  this->algo_spaceParameter();
  this->algo_fitting();
  this->algo_correction(cloud);

  //-------------------------------
  return true;
}
bool Surfacic_global_piecewise::algo_nbP(){
  nbP = 0;
  //-------------------------------

  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      nbP++;
    }
  }

  //-------------------------------
  return true;
}
bool Surfacic_global_piecewise::algo_spaceParameter(){
  //Retrieve necessary data
  refopeManager->compute_Surfacic_gloabPiecewise(Segment_1, Segment_2);
  I2048_beg.clear(); R_beg.clear(); cIt_beg.clear();
  I2048_mid.clear(); R_mid.clear(); cIt_mid.clear();
  I2048_end.clear(); R_end.clear(); cIt_end.clear();
  I01_tot.clear(); R_tot.clear(); cIt_tot.clear();
  vector<vector<float>> I01, cIt, R;
  //-------------------------------

  //Begin
  I01 = refopeManager->get_Surfacique_I01_b();
  cIt = refopeManager->get_Surfacique_cIt_b();
  R = refopeManager->get_Surfacique_R_b();
  for(int i=0; i<R.size(); i++){
    for(int j=0; j<R[i].size(); j++){
      //Piecewise vectors
      I2048_beg.push_back(I01[i][j]*2048);
      R_beg.push_back(R[i][j]);
      cIt_beg.push_back(cIt[i][j]);

      //Whole vectors
      I01_tot.push_back(I01[i][j]);
      R_tot.push_back(R[i][j]);
      cIt_tot.push_back(cIt[i][j]);
    }
  }

  //Middle
  I01 = refopeManager->get_Surfacique_I01_m();
  cIt = refopeManager->get_Surfacique_cIt_m();
  R = refopeManager->get_Surfacique_R_m();
  for(int i=0; i<R.size(); i++){
    for(int j=0; j<R[i].size(); j++){
      I2048_mid.push_back(I01[i][j]*2048);
      R_mid.push_back(R[i][j]);
      cIt_mid.push_back(cIt[i][j]);

      I01_tot.push_back(I01[i][j]);
      R_tot.push_back(R[i][j]);
      cIt_tot.push_back(cIt[i][j]);
    }
  }

  //End
  I01 = refopeManager->get_Surfacique_I01_e();
  cIt = refopeManager->get_Surfacique_cIt_e();
  R = refopeManager->get_Surfacique_R_e();
  for(int i=0; i<R.size(); i++){
    for(int j=0; j<R[i].size(); j++){
      I2048_end.push_back(I01[i][j]*2048);
      R_end.push_back(R[i][j]);
      cIt_end.push_back(cIt[i][j]);

      I01_tot.push_back(I01[i][j]);
      R_tot.push_back(R[i][j]);
      cIt_tot.push_back(cIt[i][j]);
    }
  }

  //Write data on file
  ofstream file;
  file.open ("../../data/data/IbyRbyCos(alpha)_Sphere.txt");
  file << "I "<<"R "<<"cos(alpha)"<<"\n";
  file << "--------------------------------"<<"\n";
  file <<std::fixed;
  for(int i=0; i<I01_tot.size(); i++){
    file << setprecision(5);
    file << I01_tot[i] << " " << R_tot[i] << " " << cIt_tot[i];
    file << "\n";
  }
  file.close();

  //-------------------------------
  return true;
}
bool Surfacic_global_piecewise::algo_fitting(){
  //-------------------------------

  //Compute Jacobian matrices
  MatrixXf J_b = this->compute_Jacobian(R_beg, cIt_beg);
  MatrixXf J_m = this->compute_Jacobian(R_mid, cIt_mid);;
  MatrixXf J_e = this->compute_Jacobian(R_end, cIt_end);

  //Compute surface regressions
  P_b = this->compute_SurfaceRegression(J_b, R_beg, cIt_beg, I2048_beg);
  P_m = this->compute_SurfaceRegression(J_m, R_mid, cIt_mid, I2048_mid);
  P_e = this->compute_SurfaceRegression(J_e, R_end, cIt_end, I2048_end);

  this->compute_ReferencePoints(P_b, P_m, P_e);

  //---------------------------
  return true;
}
bool Surfacic_global_piecewise::algo_correction(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  vector<int> idx;
  Ic.clear(); Im.clear();
  //---------------------------

  //Say things for debugging
  if(false){
    if(Is.size() == 0) cout<<"no Is"<<endl;
    else cout<<Is.size()<<endl;
    if(dist.size() == 0) cout<<"no dist"<<endl;
    if(cosIt.size() == 0) cout<<"no cosIt"<<endl;
    cout<<"Coeff begin: ";
    for(int j=0; j<P_b.size(); j++)cout<<P_b[j]<<" ";
    cout<<endl;
    cout<<"Coeff middle: ";
    for(int j=0; j<P_m.size(); j++)cout<<P_m[j]<<" ";
    cout<<endl;
    cout<<"Coeff end: ";
    for(int j=0; j<P_e.size(); j++)cout<<P_e[j]<<" ";
    cout<<endl;
  }

  //Compute correction for each point
  for(int i=0; i<dist.size(); i++){
    float y = dist[i];
    float x = cosIt[i];
    float fit;

    //Check for NaN and deleteted point
    if(isnan(x) == true){
      cout<<"Surface fitting: There is some NaN with cosIt"<<endl;
    }

    //Correction per segments
    if(dist[i] <= Segment_1){
      fit = multivariatePoly(P_b, x, y);
    }else if(dist[i] > Segment_1 && dist[i] <= Segment_2){
      fit = multivariatePoly(P_m, x, y);
    }else if(dist[i] > Segment_2){
      fit = multivariatePoly(P_e, x, y);
    }

    Im.push_back(fit);
    float corr = Is[i] * 2048 * (0.87*2048) / fit;
    Ic.push_back(corr);
  }

  //Reconversion and extremums
  for(int i=0; i<Im.size(); i++){
    Im[i] = (Im[i])/2048;
    Ic[i] = (Ic[i])/2048;

    if(Ic[i] < 0) Ic[i] = 0;
    if(Ic[i] > 1) Ic[i] = 1;
  }

  //---------------------------
  return true;
}

//Surface fitting
float Surfacic_global_piecewise::multivariatePoly(VectorXf P, float x, float y){
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
MatrixXf Surfacic_global_piecewise::compute_Jacobian(vector<float> R, vector<float> cIt){
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
VectorXf Surfacic_global_piecewise::compute_SurfaceRegression(MatrixXf J, vector<float> R, vector<float> cIt, vector<float> Is){
  VectorXf E = VectorXf::Zero(R.size());
  VectorXf P = VectorXf::Zero(nbP);
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
    C = A.inverse();

    P = P - C*B;
  }

  //---------------------------
  return P;
}

//Specific functions
bool Surfacic_global_piecewise::compute_ReferencePoints(VectorXf P_b, VectorXf P_m, VectorXf P_e){
  //Compute normalization coefficient
  //-------------------------------

  lambda_b = multivariatePoly(P_b, 1, 3);
  lambda_m = multivariatePoly(P_m, 1, 10.7);
  lambda_e = multivariatePoly(P_e, 1, 16);

  //-------------------------------
  return true;
}
void Surfacic_global_piecewise::compute_normalizeCoeffs(VectorXf P){
  //-------------------------------

  float last = P(P.size()-1);
  for(int i=0; i<P.size(); i++){
    P(i) = P(i) / last;
  }

  //-------------------------------
}
void Surfacic_global_piecewise::compute_error(Cloud* cloud){
  this->algo(cloud);
  vector<float> map_fit;
  float fit;
  //---------------------------------

  //Compute estimating fitting
  for(int i=0; i<R_tot.size(); i++){
    float y = R_tot[i];
    float x = cIt_tot[i];

    if(R_tot[i] <= Segment_1){
      fit = multivariatePoly(P_b, x, y);
    }else if(R_tot[i] > Segment_1 && R_tot[i] <= Segment_2){
      fit = multivariatePoly(P_m, x, y);
    }else if(R_tot[i] > Segment_2){
      fit = multivariatePoly(P_e, x, y);
    }

    map_fit.push_back(fit/2048);
  }

  //Erreur moyenne entre surface et les points.
  vector<float> err;
  for(int i=0; i<map_fit.size(); i++){
    err.push_back(abs( (I01_tot[i] - map_fit[i])/I01_tot[i] ));
  }
  cout<<"Moyenne des erreurs absolues : "<<fct_mean(err)<<endl;
  cout<<"pourcentage des erreurs absolues : "<<fct_mean(err)*100<<endl;
  cout<<"Ecart type des erreurs absolues : "<<fct_std(err)<<endl;

  //-------------------------------
}

//Plot
void Surfacic_global_piecewise::plot_SurfaceFitting(){
  Plotting plotManager;
  vector<vec3> seg1, seg2, seg3;
  vector<float> map_fit;
  float fit;
  //-------------------------------

  this->algo_nbP();
  this->algo_spaceParameter();
  this->algo_fitting();

  //Compute regression
  for(int i=0; i<R_tot.size(); i++){
    float y = R_tot[i];
    float x = cIt_tot[i];
    float z = I01_tot[i];

    if(R_tot[i] <= Segment_1){
      fit = multivariatePoly(P_b, x, y);
      seg1.push_back(vec3(x,y,z));
    }else if(R_tot[i] > Segment_1 && R_tot[i] <= Segment_2){
      fit = multivariatePoly(P_m, x, y);
      seg2.push_back(vec3(x,y,z));
    }else if(R_tot[i] > Segment_2){
      fit = multivariatePoly(P_e, x, y);
      seg3.push_back(vec3(x,y,z));
    }

    map_fit.push_back(fit/2048);
  }

  //Data in terminal
  cout<<"seg 1"<<endl;
  for(int i=0; i<seg1.size(); i++){
    cout<<seg1[i].x<<" "<<seg1[i].y<<" "<<seg1[i].z<<endl;
  }
  cout<<"seg 2"<<endl;
  for(int i=0; i<seg2.size(); i++){
    cout<<seg2[i].x<<" "<<seg2[i].y<<" "<<seg2[i].z<<endl;
  }
  cout<<"seg 3"<<endl;
  for(int i=0; i<seg3.size(); i++){
    cout<<seg3[i].x<<" "<<seg3[i].y<<" "<<seg3[i].z<<endl;
  }

  //Plotting
  plotManager.set_namePlot("3Dmap");
  plotManager.set_Xlabel("Distance R (m)");
  plotManager.set_Ylabel("cos({/Symbol a})");
  plotManager.set_Zlabel("Intensity [0;1]");
  plotManager.set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' title 'Raw data'");
  plotManager.set_Format_data2("with pm3d title 'Regression'");
  plotManager.plot_3DmapRegression(R_tot, cIt_tot, I01_tot, map_fit);

  //-------------------------------
}
void Surfacic_global_piecewise::plot_SpaceParameter(){
  Plotting plotManager;
  this->algo_spaceParameter();
  //-------------------------------

  cout<<"-- Space parameter--"<<endl;
  for(int i=0; i<I01_tot.size(); i++){
    cout<<std::fixed<<std::setprecision(5)<<I01_tot[i]<<" "<<R_tot[i]<<" "<<cIt_tot[i]<<endl;
  }

  //Plotting
  plotManager.set_namePlot("3Dmap");
  plotManager.set_Xlabel("Distance R (m)");
  plotManager.set_Ylabel("cos({/Symbol a})");
  plotManager.set_Zlabel("Intensity [0;1]");
  plotManager.set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' title 'Raw data'");
  plotManager.plot_3DmapSpaceParameter(R_tot, cIt_tot, I01_tot);

  //-------------------------------
}
void Surfacic_global_piecewise::plot_intensityCorrection(Cloud* cloud){
  Plotting plotManager;
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  this->algo(cloud);
  //--------------------------------

  plotManager.plot_Curve_lin(Is, Im);
  plotManager.plot_2histograms(Is, Ic);

  //-------------------------------
}
