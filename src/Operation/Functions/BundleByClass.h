#ifndef BundleByClass_H
#define BundleByClass_H

#include "gnuplot/gnuplot-iostream.h"

#include "../../common.h"

class BundleByClass
{
public:
  //Constructor / Destructor
  BundleByClass();
  ~BundleByClass();

public:
  //Bundle functions
  void compute_bundleByClass(Cloud* cloud, float step);
  void compute_bundleByClass_maxAngle(Cloud* cloud_in, int stepAngle, int maxAngle);
  void compute_bundleByClass_vector(vector<float>& It, vector<float>& Is, int stepAngle);
  void compute_bundleByClass_vector2(vector<float>& It, vector<float>& cosIt, vector<float>& Is, float stepAngle, float maxAngle);
  void compute_vectorByClass(vector<vector<float>>& data_X, vector<vector<float>>& data_Y);

  //Subfunctions
  void make_checking(Cloud* cloud);
  void make_clear();
  void make_bundle(Cloud* cloud, float stepAngle, int maxAngle);
  void make_StudentLaw();

  //Plot functions
  void plot_confidenceInterval();
  void plot_intensityBundle(Cloud* cloud);

  //Setters / Getters
  inline vector<float> get_Ib(){return Ib;}
  inline vector<float> get_Ib_dist(){return Ib_dist;}
  inline vector<float> get_Ib_It(){return Ib_It;}
  inline vector<float> get_Ib_It_norm(){return Ib_It_norm;}
  inline vector<float> get_Ib_cosIt(){return Ib_cosIt;}
  inline vector<float> get_Ib_std(){return Ib_std;}
  inline vector<float> get_Ib_nD(){return Ib_nD;}
  inline vector<float> get_Ib_Jk(){return Ib_Jk;}

  inline vector<float> get_vecb_dX(){return vecb_dX;}
  inline vector<float> get_vecb_dY(){return vecb_dY;}
  inline vector<float> get_vecb_std(){return vecb_std;}
  inline vector<float> get_vecb_nD(){return vecb_nD;}
  inline vector<vector<float>> get_data_X_inv(){return data_X_inv;}
  inline vector<vector<float>> get_data_Y_inv(){return data_Y_inv;}

private:
  vector<float> Ib;
  vector<float> Ib_nD, Ib_dist;
  vector<float> Ib_It, Ib_It_norm, Ib_cosIt;
  vector<float> Ib_std, Ib_Jk;

  vector<float> vecb_dX, vecb_dY, vecb_std, vecb_nD;
  vector<vector<float>> data_X_inv, data_Y_inv;
};

#endif
