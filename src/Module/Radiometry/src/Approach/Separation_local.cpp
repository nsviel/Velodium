#include "Separation_local.h"

#include "../Target/Ref_Operation.h"

#include "../../../../Operation/Function/Plotting.h"
#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/SpeudoInverse.h"
#include "../../../../Specific/fct_math.h"

#include <flann/flann.hpp>


//Constructor / Destructor
Separation_local::Separation_local(Ref_Operation* opeClass){
  this->refopeManager = opeClass;
  //---------------------------

  bundler = new BundleByClass();
  plotManager = new Plotting();

  this->n = 3; //R
  this->m = 2; //It
  this->knn = 20;
  this->iter = 5;
  this->lambda = 1;
  this->precision_angle = 0.5;
  this->plotting = false;

  //---------------------------
}
Separation_local::~Separation_local(){}

//Method function
bool Separation_local::algo(Cloud* cloud){
  //---------------------------

  this->algo_ParameterSpace();
  this->algo_checking(cloud);
  this->algo_interpolation(cloud);
  this->algo_correction(cloud);

  //---------------------------
  return true;
}

//Sub-functions
bool Separation_local::algo_checking(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  vector<float>& It = subset->It;
  //---------------------------

  if(Is.size() == 0){
    cout<<"No intensity data"<<endl;
  }
  if(dist.size() == 0){
    cout<<"No distance data"<<endl;
  }
  if(cosIt.size() == 0){
    cout<<"No cosIt data"<<endl;
  }
  if(It.size() == 0){
    cout<<"No It data"<<endl;
  }
  if(PS_Sphere.size() == 0){
    cout<<"Parameter space is empty"<<endl;
  }

  //---------------------------
  return true;
}
bool Separation_local::algo_ParameterSpace(){
  PS_Sphere.clear(); PS_Spectralon.clear();
  nbP = compute_nbP();
  //----------------------

  //Parameter space = vector<vec3> : [Is][cosIt][dist]
  if(refopeManager->compute_ParameterSpace_Sphere(precision_angle)){
    PS_Sphere = refopeManager->get_ParameterSpace();
  }

  //----------------------
  PS_size = PS_Sphere.size();
  return true;
}
bool Separation_local::algo_interpolation(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  vector<float>& dist = subset->R;
  vector<float>& cosIt = subset->cosIt;
  vector<float>& It = subset->It;
  Im.clear();
  //---------------------------------

  //kNN search
  vector<vec2> PT_PS, PT_query;
  for(int i=0; i<PS_Sphere.size(); i++){
    float R = PS_Sphere[i].z;
    float It = acos(PS_Sphere[i].y) * 180 / M_PI;
    PT_PS.push_back(vec2(R, It));
  }
  for(int i=0; i<Is.size(); i++){
    float R = dist[i];
    float It = acos(cosIt[i]) * 180 / M_PI;
    PT_query.push_back(vec2(R, It));
  }
  MatrixXf list_kNN = kNN_KdTreeFLANN(PT_PS, PT_query, true);

  //Surface regression
  MatrixXf list_P = compute_SurfaceRegression(PS_Sphere, list_kNN);

  //Fitting
  for(int i=0; i<Is.size(); i++){
    float It = acos(cosIt[i]) * 180 / M_PI;
    float fit = multivariatePoly(list_P.row(i), dist[i], It);
    Im.push_back(fit*2048);

    //Plotting regression on point
    if(plotting && i<20){
      std::ostringstream sd, sc;
      sd << dist[i];
      sc << cosIt[i];
      std::string ssd(sd.str());
      std::string ssc(sc.str());
      string title = "R: " + ssd + " | cIt: " + ssc;
      this->plot_SurfaceRegression(list_P.row(i), PS_Sphere, list_kNN.row(i), title);
    }
  }

  //---------------------------------
  if(Im.size() != Is.size()){
    cout<<"Problem interpolation Im size"<<endl;
    return false;
  }
  return true;
}
bool Separation_local::algo_correction(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  vector<float>& Is = subset->I;
  Ic.clear();
  //---------------------------

  //Intensity correction
  for(int i=0; i<Is.size(); i++){
    float Icorr = Is[i] * 2048 * (0.87*2048) / (Im[i]);
    Ic.push_back(Icorr);
  }
  //Intensity formatting
  for(int i=0; i<Is.size(); i++){
    Im[i] = (Im[i])/2048;
    Ic[i] = (Ic[i])/2048;

    if(Ic[i] < 0) Ic[i] = 0;
    if(Ic[i] > 1) Ic[i] = 1;
  }

  //---------------------------
  if(Ic.size() != Is.size()){
    cout<<"Problem interpolation Ic size"<<endl;
    return false;
  }
  return true;
}

//Math functions
MatrixXf Separation_local::kNN_KdTreeFLANN(vector<vec2> PT_PS, vector<vec2> PT_query, bool normalized){
  int nb_iter = 32;
  //-------------------------

  //Normalization
  if(normalized){
    vec2 min = fct_min_vec2(PT_PS);
    vec2 max = fct_max_vec2(PT_PS);

    //Target normalization
    for(int i=0; i<PT_PS.size(); i++){
      PT_PS[i][0] = (PT_PS[i][0] - min[0]) / (max[0] - min[0]);
      PT_PS[i][1] = (PT_PS[i][1] - min[1]) / (max[1] - min[1]);
    }

    //Query normalization
    for(int i=0; i<PT_query.size(); i++){
      PT_query[i][0] = (PT_query[i][0] - min[0]) / (max[0] - min[0]);;
      PT_query[i][1] = (PT_query[i][1] - min[1]) / (max[1] - min[1]);
    }
  }

  //Create FLANN matrices input
  flann::Matrix<float> dataset = flann::Matrix<float>(&PT_PS[0][0], PT_PS.size(), 2);
  flann::Matrix<float> query = flann::Matrix<float>(&PT_query[0][0], PT_query.size(), 2);

  //Construct an randomized kd-tree index using 4 kd-trees
  flann::Index<flann::L2<float>> index(dataset, flann::KDTreeSingleIndexParams(10));
  index.buildIndex();

  //Create index matrices output
  vector<vector<int>> indices(query.rows, vector<int>(knn,0));
  vector<vector<float>> dists(query.rows, vector<float>(knn,0));
  flann::SearchParams params;
  params.cores = 0;
  params.checks = nb_iter;
  index.knnSearch(query, indices, dists, knn, params);

  //Create vector of index
  MatrixXf list_kNN(indices.size(), knn);
  for(int i=0; i<indices.size(); i++){
    for(int j=0; j<knn; j++){
      list_kNN(i,j) = indices[i][j];
    }
  }

  //-------------------------
  return list_kNN;
}
MatrixXf Separation_local::compute_SurfaceRegression(vector<vec3>& PS, MatrixXf list_kNN){
  MatrixXf list_P(list_kNN.rows(), nbP);
  SpeudoInverse invManager;
  //---------------------------

  //For each point kNN
  for(int i=0; i<list_kNN.rows(); i++){

    //Jacobian & Speudo-inverse
    VectorXf kNN = list_kNN.row(i);
    MatrixXf J = compute_Jacobian(PS, kNN);
    MatrixXf Jinv = invManager.SpeudoInverse_orthoDecomp(J);

    //Fitting surface
    VectorXf P = VectorXf::Zero(nbP);
    VectorXf E = VectorXf::Zero(kNN.size());

    //---> iterations
    for(int k=0; k<iter; k++){
      for(int j=0; j<kNN.size(); j++){
        float x = PS[kNN(j)][2]; //R
        float y = PS[kNN(j)][1]; //cIt
        float z = PS[kNN(j)][0]; //Is
        y = acos(y) * 180 / M_PI;

        //---> Fitting point
        float fit = multivariatePoly(P, x, y);

        //---> Error  vectors
        E(j) = fit - z;
      }

      //Optimization
      P = P - lambda*(Jinv*E);
    }

    list_P.row(i) = P;
  }

  //---------------------------
  return list_P;
}
MatrixXf Separation_local::compute_Jacobian(vector<vec3>& PS, VectorXf kNN){
  MatrixXf J = MatrixXf::Zero(kNN.size(), nbP);
  //--------------------------

  for(int k=0; k<kNN.size(); k++){
    float x = PS[kNN(k)][2];
    float y = PS[kNN(k)][1];
    y = acos(y) * 180 / M_PI;

    int cpt = 0;
    for(int i=0; i<=m; i++){
      for(int j=0; j<=n; j++){
        J(k, cpt) = pow(x,j)*pow(y,i);
        cpt++;
      }
    }
  }

  //---------------------------
  return J;
}
float Separation_local::multivariatePoly(VectorXf P, float x, float y){
  float fit = 0;
  int cpt = 0;
  //--------------------------

  for(int i=0; i<=m; i++){ //y
    for(int j=0; j<=n; j++){ //x
      fit += P(cpt)*pow(x,j)*pow(y,i);
      cpt++;
    }
  }

  //--------------------------
  return fit;
}
float Separation_local::compute_nbP(){
  nbP = 0;
  //---------------------------

  for(int i=0; i<=m; i++){
    for(int j=0; j<=n; j++){
      nbP++;
    }
  }

  //---------------------------
  return nbP;
}

//Plotting
void Separation_local::plot_ParameterSpace(){
  this->algo_ParameterSpace();
  this->plot_normalizedRA();
  this->plot_RA();
  //---------------------------

  //Plotting
  plotManager->set_namePlot("3Dmap");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("cos({/Symbol a})");
  plotManager->set_Zlabel("Intensity [0;1]");
  plotManager->set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' title 'Raw data'");
  if(PS_Sphere.size() != 0){
    plotManager->plot_3Dmap_vec3(PS_Sphere);
  }

  //---------------------------
}
void Separation_local::plot_RA(){
  this->algo_ParameterSpace();
  vector<vec3> PS = PS_Sphere;
  vector<float> R, A, It;
  //---------------------------

  //Get data
  float min, max;
  for(int i=0; i<PS_size; i++){
    R.push_back(PS[i].z);
    A.push_back(PS[i].y);
    It.push_back( acos(PS[i].y) * 180 / M_PI );
  }

  //Write in terminal
  cout<<"fct_normalized parameter space"<<endl;
  for(int i=0; i<R.size(); i++){
    cout<<R[i]<<" "<<It[i]<<endl;
  }

  //Plotting
  plotManager->set_namePlot("(R,cos(It))");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("cos({/Symbol a})");
  plotManager->set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' notitle");
  plotManager->plot_Curve(R, It);

  //---------------------------
}
void Separation_local::plot_normalizedRA(){
  this->algo_ParameterSpace();
  vector<vec3> PS = PS_Sphere;
  vector<float> R, A, It;
  //---------------------------

  //Get data
  float min, max;
  for(int i=0; i<PS_size; i++){
    R.push_back(PS[i].z);
    A.push_back(PS[i].y);
    It.push_back( acos(PS[i].y) * 180 / M_PI );
  }

  //Normalization
  //R norm
  min = R[0]; max = R[0];
  for(int i=0; i<R.size(); i++){
    if(R[i] > max) max = R[i];
    if(R[i] < min) min = R[i];
  }
  for(int i=0; i<R.size(); i++){
    R[i] = (R[i] - min) / (max - min);
  }
  //CosIt norm
  min = A[0]; max = A[0];
  for(int i=0; i<A.size(); i++){
    if(A[i] > max) max = A[i];
    if(A[i] < min) min = A[i];
  }
  for(int i=0; i<A.size(); i++){
    A[i] = (A[i] - min) / (max - min);
  }
  //It norm
  min = It[0]; max = It[0];
  for(int i=0; i<A.size(); i++){
    if(It[i] > max) max = It[i];
    if(It[i] < min) min = It[i];
  }
  for(int i=0; i<A.size(); i++){
    It[i] = (It[i] - min) / (max - min);
  }

  //Write in terminal
  cout<<"fct_normalized parameter space"<<endl;
  for(int i=0; i<R.size(); i++){
    cout<<R[i]<<" "<<It[i]<<endl;
  }

  //Plotting
  plotManager->set_namePlot("(R,cos(It))");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("cos({/Symbol a})");
  plotManager->set_Format_data1("with points pt 7 ps 0.5 lc rgb 'black' notitle");
  plotManager->plot_Curve(R, It);

  //---------------------------
}
void Separation_local::plot_SurfaceRegression(VectorXf P, vector<vec3>& PS, VectorXf kNN, string t1){
  vector<float> map_R, map_cIt, map_I, map_Ifit;
  //---------------------------------

  for(int j=0; j<kNN.size(); j++){
    float x = PS[kNN(j)][2]; //R
    float y = PS[kNN(j)][1]; //cIt
    float z = PS[kNN(j)][0]; //Is
    float It = acos(y) * 180 / M_PI ;

    float fit = multivariatePoly(P, x, It);

    map_R.push_back(x);
    map_cIt.push_back(y);
    map_I.push_back(z);
    map_Ifit.push_back(fit);
  }

  //Write in terminal
  cout<<"--> Next point"<<endl;
  for(int i=0; i<map_R.size(); i++){
    float It = acos(map_cIt[i]) * 180 / M_PI ;
    cout<<map_R[i]<<" "<<It<<" "<<map_I[i]<<" "<<map_Ifit[i]<<endl;
  }

  //Plotting
  plotManager->set_namePlot("3Dmap");
  plotManager->set_Xlabel("Distance R (m)");
  plotManager->set_Ylabel("cos({/Symbol a})");
  plotManager->set_Zlabel("Intensity [0;1]");
  plotManager->set_zRange("[*:*]");
  plotManager->plot_3DmapRegression_title(map_R, map_cIt, map_I, map_Ifit, t1);

  //---------------------------
}
