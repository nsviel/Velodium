#include "BundleByClass.h"

#include "../../Specific/fct_maths.h"


//Constructor / Destructor
BundleByClass::BundleByClass(){}
BundleByClass::~BundleByClass(){}

//Bundle functions
void BundleByClass::compute_bundleByClass(Cloud* cloud, float stepAngle){
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  this->make_checking(cloud);
  this->make_clear();

  //Compute bundling
  this->make_bundle(cloud, stepAngle, fct_max(subset_init->It));

  //Compute confidence interval
  this->make_StudentLaw();
  //---------------------------
}
void BundleByClass::compute_bundleByClass_maxAngle(Cloud* cloud, int stepAngle, int maxAngle){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  vector<float>& Is = subset->I;
  vector<float>& dist = subset_init->R;
  vector<float>& cosIt = subset_init->cosIt;
  vector<float>& It = subset_init->It;
  vector<float> cIs, cDist, cCosIt, cIt;
  this->make_checking(cloud);
  this->make_clear();

  //Sample of data for each degree units
  for(float i=0; i<maxAngle; i+=stepAngle){
    //init
    int nD = 0;
    cIs.clear();
    cDist.clear();
    cCosIt.clear();

    //Create one class
    for(int j=0; j<It.size(); j++){
      if(It[j] >= i && It[j] < (i + stepAngle)){
        nD++;
        //say(j);say(Is[j]);
        cIs.push_back(Is[j]);
        cDist.push_back(dist[j]);
        cCosIt.push_back(cosIt[j]);
        cIt.push_back(It[j]);
      }
    }

    //Store class means
    if(nD != 0){
      //say(fct_mean(cIs));
      Ib.push_back(fct_mean(cIs));
      Ib_nD.push_back(nD);
      Ib_dist.push_back(fct_mean(cDist));
      Ib_It.push_back(fct_mean(cIt));
      Ib_cosIt.push_back(fct_mean(cCosIt));
      Ib_std.push_back(fct_std(cIs));
    }
  }

  //---------------------------
  if(Ib.size() == 0){
    cout<<"--> Bundle by class: fail"<<endl;
  }
}
void BundleByClass::compute_bundleByClass_vector(vector<float>& It, vector<float>& Is, int stepAngle){
  this->make_clear();
  //---------------------------

  //Make bundle
  vector<float> cIs, cIt;
  for(int i=0; i<fct_max(It); i=i+stepAngle){
     //Sample of data for each degree units
     cIs.clear();
     cIt.clear();
     int nD = 0;

     for(int j=0; j<It.size(); j++){
       if(It[j] >= i && It[j] < (i + stepAngle)){
           cIs.push_back(Is[j]);
           cIt.push_back(It[j]);
           nD++;
       }
     }

    //Store bundled intensity
    if(nD != 0){
      Ib.push_back(fct_mean(cIs));
      Ib_nD.push_back(nD);
      Ib_It.push_back(fct_mean(cIt));
      Ib_std.push_back(fct_std(cIs));
    }
  }

  //---------------------------
}
void BundleByClass::compute_bundleByClass_vector2(vector<float>& It, vector<float>& cosIt, vector<float>& Is, float stepAngle, float maxAngle){
  this->make_clear();
  if(maxAngle == 0) maxAngle = fct_max(It);
  //---------------------------

  //Make bundle
  vector<float> cIs, cIt, cCosIt;
  for(float i=0; i<maxAngle; i=i+stepAngle){
     //Sample of data for each degree units
     cIs.clear();
     cIt.clear();
     cCosIt.clear();
     int nD = 0;

     for(float j=0; j<It.size(); j++){
       if(It[j] >= i && It[j] < (i + stepAngle)){
           cIs.push_back(Is[j]);
           cIt.push_back(It[j]);
           cCosIt.push_back(cosIt[j]);
           nD++;
       }
     }

    //Store bundled intensity
    if(nD != 0){
      Ib.push_back(fct_mean(cIs));
      Ib_nD.push_back(nD);
      Ib_cosIt.push_back(fct_mean(cCosIt));
      Ib_It.push_back(fct_mean(cIt));
      Ib_It_norm.push_back(i + stepAngle/2);
      Ib_std.push_back(fct_std(cIs));
    }
  }

  //---------------------------
}
void BundleByClass::compute_vectorByClass(vector<vector<float>>& data_X, vector<vector<float>>& data_Y){
  this->make_clear();
  //---------------------------

  //Transpose a vector of vector into a vector of classes
  vector<vector<float>> cData_X, cData_Y;
  vector<float> cb_X, cb_Y;

  for(int j=0; j<fct_max_vec(data_X); j++){
    cb_X.clear();
    cb_Y.clear();

    for(int i=0; i<data_X.size(); i++){
      if(j < data_X[i].size()){
        cb_X.push_back(data_X[i][j]);
      }
      if(j < data_Y[i].size()){
        cb_Y.push_back(data_Y[i][j]);
      }
    }

    cData_X.push_back(cb_X);
    cData_Y.push_back(cb_Y);
  }

  data_X_inv = cData_X;
  data_Y_inv = cData_Y;

  //For each class compute data
  for(int i=0; i<cData_X.size(); i++){
    vecb_dX.push_back(fct_mean(cData_X[i]));
    vecb_dY.push_back(fct_mean(cData_Y[i]));
    vecb_std.push_back(fct_std(cData_Y[i]));
    vecb_nD.push_back(cData_Y[i].size());
  }

  //---------------------------
}

//Subfunctions
void BundleByClass::make_bundle(Cloud* cloud, float stepAngle, int maxAngle){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  vector<float>& Is = subset->I;
  vector<float>& dist = subset_init->R;
  vector<float>& cosIt = subset_init->cosIt;
  vector<float>& It = subset_init->It;
  vector<float> cIs, cDist, cCosIt, cIt;

  //Sample of data for each degree units
  for(float i=0; i<maxAngle; i=i+stepAngle){
    int nD = 0;
    cIs.clear();
    cDist.clear();
    cCosIt.clear();
    cIt.clear();

    for(int j=0; j<It.size(); j++){
     if(It[j] >= i && It[j] < (i + stepAngle)){
       nD++;

       cIs.push_back(Is[j]);
       cDist.push_back(dist[j]);
       cCosIt.push_back(cosIt[j]);
       cIt.push_back(It[j]);
     }
    }

    //Store bundled intensity
    if(nD != 0){
      Ib.push_back(fct_mean(cIs));
      Ib_nD.push_back(nD);
      Ib_dist.push_back(fct_mean(cDist));
      Ib_It.push_back(fct_mean(cIt));
      Ib_cosIt.push_back(fct_mean(cCosIt));
      Ib_std.push_back(fct_std(cIs));
    }
  }

  //---------------------------
  if(Ib.size() == 0){
    cout<<"--> Bundle by class: fail"<<endl;
  }
}
void BundleByClass::make_checking(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  vector<float>& Is = subset->I;
  vector<float>& dist = subset_init->R;
  vector<float>& cosIt = subset_init->cosIt;
  vector<float>& It = subset_init->It;

  if(Is.size() == 0){
    cout<<"--> Bundle: No intensity data"<<endl;
  }
  if(dist.size() == 0){
    cout<<"--> Bundle: No distance data"<<endl;
  }
  if(cosIt.size() == 0){
    cout<<"--> Bundle: No cosIt data"<<endl;
  }
  if(It.size() == 0){
    cout<<"--> Bundle: No It data"<<endl;
  }

  //---------------------------
}
void BundleByClass::make_clear(){
  //---------------------------

  //Clear lists
  Ib.clear();
  Ib_dist.clear();
  Ib_cosIt.clear();
  Ib_It.clear();
  Ib_std.clear();
  Ib_nD.clear();
  Ib_Jk.clear();
  Ib_It_norm.clear();

  vecb_dX.clear();
  vecb_dY.clear();
  vecb_std.clear();
  vecb_nD.clear();
  data_X_inv.clear();
  data_Y_inv.clear();

  //---------------------------
}
void BundleByClass::make_StudentLaw(){
  //---------------------------

  for(int i=0; i<Ib_nD.size(); i++){
    //tk determination
    float tk;
    if(Ib_nD[i] > 1){
      tk = 1.96;//tinv(0.975, Ib_nD[i]-1);
    }
    else{
      tk = 12.71;
    }

    //Jk determination
    vec2 Jk;
    if(Ib_nD[i] > 1){
      Jk.x = Ib[i] - tk * Ib_std[i] / sqrt(Ib_nD[i] - 1);
      Jk.y = Ib[i] + tk * Ib_std[i] / sqrt(Ib_nD[i] - 1);
    }
    else{
      Jk.x = Ib[i];
      Jk.y = Ib[i];
    }

    //Convert to confidente intervale percent
    float Jk_perc = (1 - (Ib[i] / Jk.y)) * 100;

    //increment vector data
    Ib_Jk.push_back(Jk_perc);
  }

  //---------------------------
}

//Plot functions
void BundleByClass::plot_confidenceInterval(){
    Gnuplot gp;
    //---------------------------

    //Style
    gp << "set grid\n"; //Grid
    gp << "set key opaque box lt -1 lw 2\n";
    gp << "set style line 11 lc rgb '#606060' lt 1\n";
    gp << "set border 3 back ls 11\n";
    gp << "set tics nomirror\n"; //Supress top and rigth tics
    gp << "set xlabel 'cos(It)'\n";
    gp << "set ylabel 'Confidence interval'\n";

    //Data
    gp << "plot '-' with linespoints pt 7 ps 0.5 lc rgb 'black' smooth unique notitle\n";
    gp.send1d(std::tuple(Ib_cosIt, Ib_Jk));

    //Range
    gp << "set xrange ["<<fct_min(Ib_cosIt)<<":"<<fct_max(Ib_cosIt)<<"]\n";
    gp << "set yrange [0:10]\n";

    //---------------------------
}
void BundleByClass::plot_intensityBundle(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  vector<float>& Is = subset->I;
  vector<float>& It = subset_init->It;
  Gnuplot gp;

  //Style
  gp << "set grid\n"; //Grid
  gp << "set key opaque box lt -1 lw 2\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel 'cos({/Symbol a})'\n";
  gp << "set ylabel 'I_{raw}'\n";

  //Data
  gp << "plot '-' with yerrorbars pt 7 ps 1 title 'Class', ";
  gp << "'-' with points title 'Raw'\n";
  gp.send1d(std::tuple(Ib_It, Ib, Ib_std));
  gp.send1d(std::tuple(It, Is));

  //Range
  gp << "set xrange ["<<fct_min(Ib_It)<<":"<<fct_max(Ib_It)<<"]\n";
  gp << "set yrange ["<<fct_min(Ib)<<":"<<fct_max(Ib)<<"]\n";

  //---------------------------
}
