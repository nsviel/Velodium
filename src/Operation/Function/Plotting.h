#ifndef PLOTTING_H
#define PLOTTING_H

#include "../../../extern/gnuplot/gnuplot-iostream.h"

#include "../../common.h"
#include <Eigen/Dense>

using namespace Eigen;


class Plotting
{
public:
  //Constructor / Destructor
  Plotting();
  ~Plotting();

public:
  void Clear();

  void plot_Curve(vector<float>& Xd, vector<float>& Yd);
  void plot_Curve_discret(vector<int>& Xd, vector<float>& Yd);
  void plot_CurveError(vector<float>& Xd, vector<float>& Yd, vector<float>& err);
  void plot_CurveError_cross(vector<float>& Xd, vector<float>& Yd, vector<float>& err_v, vector<float>& err_h);
  void plot_CurveErrorLine(vector<float>& Xd, vector<float>& Yd, vector<float>& err);
  void plot_2Curves(vector<float>& X1d, vector<float>& Y1d, vector<float>& X2d, vector<float>& Y2d);
  void plot_2CurvesError(vector<float>& X1d, vector<float>& Y1d, vector<float>& err, vector<float>& X2d, vector<float>& Y2d);

  void plot_Curve_Multi(vector<vector<float>>& Xd, vector<vector<float>>& Yd);
  void plot_Curve_Multi_crossErr(vector<vector<float>>& Xd, vector<vector<float>>& Yd, vector<vector<float>>& err_v, vector<vector<float>>& err_h);
  void plot_Curve_Multi_Err(vector<vector<float>>& Xd, vector<vector<float>>& Yd, vector<vector<float>>& err);
  void plot_Curve_lin(vector<float>& Xd, vector<float>& Yd);
  void plot_Regression(vector<float>& Xd, vector<float> &Y1d, vector<float>& Y2d);
  void plot_MultipleCurveRegression(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& Y3d, vector<float>& Y4d, vector<float>& regr);
  void plot_histogram(vector<float>& Xd);
  void plot_2histograms(vector<float>& X1d, vector<float>& X2d);
  void plot_PointCloud(Cloud* cloud);
  void plot_CurveErrorRegression(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& err);
  void plot_3Dmap_vec3(vector<vec3> Xd);
  void plot_3Dmap(vector<vector<float>>& Xd, vector<vector<float>>& Yd, vector<vector<float>>& Zd);
  void plot_3DmapRegression(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su);
  void plot_3DmapRegression_title(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su, string t1);
  void plot_3DmapRegression_points(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su);
  void plot_3DmapRegression_mat(MatrixXf& mat);
  void plot_3DmapOnlyRegression(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_su);
  void plot_2Dmap(MatrixXf heatmap, vector<float>& R_map, vector<float>& cosIt_map);
  void curve_Spectralon();
  void plot_2Regression(vector<float>& Xd1, vector<float>& Y1d1, vector<float>& Y2d1, vector<float>& Y1d2, vector<float>& Y2d2);
  void plot_3DmapSpaceParameter(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt);
  void plot_MultipleCurveRegression_tot(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& Y3d, vector<float>& Y4d, vector<float>& Reg1, vector<float>& Reg2, vector<float>& Reg3, vector<float>& Reg4);

  inline void set_namePlot(string value){this->namePlot = value;}
  inline void set_Xlabel(string value){this->Xlabel = value;}
  inline void set_Ylabel(string value){this->Ylabel = value;}
  inline void set_Zlabel(string value){this->Zlabel = value;}
  inline void set_xRange(string value){this->xRange = value;}
  inline void set_yRange(string value){this->yRange = value;}
  inline void set_zRange(string value){this->zRange = value;}
  inline void set_dataForm(vector<string> value){this->dataForm = value;}
  inline void set_dataFormAdd(vector<string> value){this->dataFormAdd = value;}
  inline void set_dataTitle(vector<string> value){this->dataTitle = value;}
  inline void set_dataColor(vector<string> value){this->dataColor = value;}
  inline void set_dataFormA(vector<string> value){this->dataFormA = value;}
  inline void set_Format_data1(string value){this->format_data1 = value;}
  inline void set_Format_data2(string value){this->format_data2 = value;}
  inline void set_keyOn(bool value){this->keyOn = value;}
  inline void set_keyOutside(bool value){this->keyOutside = value;}
  inline void set_labelOn(bool value){this->labelOn = value;}
  inline void set_ticSize(int value){this->ticSize = to_string(value);}

private:
  bool flag;
  bool keyOn;
  bool keyOutside;
  bool labelOn;
  string ticSize;
  string namePlot;
  string Xlabel, Ylabel, Zlabel;
  string format_data1, format_data2;
  string xRange, yRange, zRange;
  vector<string> dataColor;
  vector<string> dataForm, dataFormA;
  vector<string> dataFormAdd;
  vector<string> dataTitle;
};

#endif
