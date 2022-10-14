#include "Plotting.h"

#include "../../Specific/color.h"
#include "../../Specific/fct_maths.h"


//Constructor / Destructor
Plotting::Plotting(){
  //---------------------------

  this->keyOn = true;
  this->keyOutside = true;
  this->labelOn = true;
  this->ticSize = "10";

  //---------------------------
  this->Clear();
}
Plotting::~Plotting(){}

//Plotting functions
void Plotting::Clear(){
  //---------------------------

  this->namePlot.clear();
  this->Xlabel.clear();
  this->Ylabel.clear();
  this->Ylabel.clear();

  this->dataColor.clear();
  this->dataForm.clear();
  this->dataFormAdd.clear();
  this->dataTitle.clear();

  this->xRange = "[*:*]";
  this->yRange = "[*:*]";
  this->zRange = "[*:*]";

  if(namePlot.empty()) this->namePlot = "Plot";
  if(Xlabel.empty()) this->Xlabel = "X";
  if(Ylabel.empty()) this->Ylabel = "Y";
  if(Ylabel.empty()) this->Ylabel = "Z";

  this->flag = true;

  //---------------------------
}

//Curves
void Plotting::plot_Curve(vector<float>& Xd, vector<float>& Yd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' "+ format_data1+"\n";
  gp.send1d(std::tuple(Xd, Yd));

  //---------------------------
  this->Clear();
}
void Plotting::plot_Curve_discret(vector<int>& Xd, vector<float>& Yd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' with linespoints pt 7 ps 0.5 lc rgb 'black'\n";
  gp.send1d(std::tuple(Xd, Yd));

  //---------------------------
  this->Clear();
}
void Plotting::plot_CurveError(vector<float>& Xd, vector<float>& Yd, vector<float>& err){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set term wxt enhanced font ',10'\n";
  gp << "set grid\n"; //Grid
  if(keyOutside == true) gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' with yerrorlines "+ format_data1 +"\n";
  gp.send1d(std::tuple(Xd, Yd, err));

  //---------------------------
  this->Clear();
}
void Plotting::plot_CurveError_cross(vector<float>& Xd, vector<float>& Yd, vector<float>& err_v, vector<float>& err_h){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' with xyerrorbars "+ format_data1+"\n";
  gp.send1d(std::tuple(Xd, Yd, err_v, err_h));

  //---------------------------
  this->Clear();
}
void Plotting::plot_CurveErrorLine(vector<float>& Xd, vector<float>& Yd, vector<float>& err){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //--------------------------

  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' with lines "+ format_data1 +", ";
  gp << "'-' with yerrorbars "+ format_data2 +"\n";
  gp.send1d(std::tuple(Xd, Yd));
  gp.send1d(std::tuple(Xd, Yd, err));

  //---------------------------
  this->Clear();
}
void Plotting::plot_2Curves(vector<float>& X1d, vector<float>& Y1d, vector<float>& X2d, vector<float>& Y2d){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";

  auto minY=0, maxY=1;
  if(fct_min(Y1d) < fct_min(Y2d)) minY = fct_min(Y1d); else minY = fct_min(Y2d);
  if(fct_max(Y1d) > fct_max(Y2d)) maxY = fct_max(Y1d); else maxY = fct_max(Y2d);
  gp << "set xrange ["<<fct_min(X1d)<<":"<<fct_max(X1d)<<"]\n";
  gp << "set yrange ["<<minY<<":"<<maxY<<"]\n";

  gp << "plot '-' "+ format_data1 +", ";
  gp << "'-' "+ format_data2 +"\n";
  gp.send1d(std::tuple(X1d, Y1d));
  gp.send1d(std::tuple(X2d, Y2d));

  //---------------------------
  this->Clear();
}
void Plotting::plot_2CurvesError(vector<float>& X1d, vector<float>& Y1d, vector<float>& err, vector<float>& X2d, vector<float>& Y2d){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";

  auto minY=0, maxY=1;
  if(fct_min(Y1d) < fct_min(Y2d)) minY = fct_min(Y1d); else minY = fct_min(Y2d);
  if(fct_max(Y1d) > fct_max(Y2d)) maxY = fct_max(Y1d); else maxY = fct_max(Y2d);
  gp << "set xrange ["<<fct_min(X1d)<<":"<<fct_max(X1d)<<"]\n";
  gp << "set yrange ["<<minY<<":"<<maxY<<"]\n";

  gp << "plot '-' with yerrorlines "+ format_data1 +", ";
  gp << "'-' "+ format_data2 +"\n";
  gp.send1d(std::tuple(X1d, Y1d, err));
  gp.send1d(std::tuple(X2d, Y2d));

  //---------------------------
  this->Clear();
}
void Plotting::plot_Curve_Multi(vector<vector<float>>& Xd, vector<vector<float>>& Yd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n";
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n";
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot ";
  for(int i=0; i<Xd.size(); i++){
    gp << "'-' with lines lc rgb 'black' notitle";

    if(i < Xd.size()-1){
      gp << ", ";
    }
  }
  gp << "\n";
  for(int i=0; i<Xd.size(); i++){
    gp.send1d(std::tuple(Xd[i], Yd[i]));
  }

  //---------------------------
  this->Clear();
}
void Plotting::plot_Curve_Multi_crossErr(vector<vector<float>>& Xd,
  vector<vector<float>>& Yd, vector<vector<float>>& err_v, vector<vector<float>>& err_h){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n";
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n";
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot ";
  for(int i=0; i<Xd.size(); i++){
    gp << "'-' with xyerrorbars "+ format_data1;

    if(i < Xd.size()-1){
      gp << ", ";
    }
  }
  gp << "\n";
  for(int i=0; i<Xd.size(); i++){
    gp.send1d(std::tuple(Xd[i], Yd[i], err_v, err_h));
  }

  //---------------------------
  this->Clear();
}
void Plotting::plot_Curve_Multi_Err(vector<vector<float>>& Xd, vector<vector<float>>& Yd, vector<vector<float>>& err){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  if(keyOutside == true) gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n";
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot ";
  for(int i=0; i<Xd.size(); i++){
    gp << "'-' with yerrorlines "+ dataFormA[i];

    if(i < Xd.size()-1){
      gp << ", ";
    }
  }
  gp << "\n";
  for(int i=0; i<Xd.size(); i++){
    gp.send1d(std::tuple(Xd[i], Yd[i], err[i]));
  }

  //---------------------------
  this->Clear();
}
void Plotting::plot_Curve_lin(vector<float>& Xd, vector<float>& Yd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel 'Raw intensity'\n";
  gp << "set ylabel 'Corrected intensity'\n";
  gp << "set xrange ["<<fct_min(Xd)<<":"<<fct_max(Xd)<<"]\n";
  gp << "set yrange ["<<fct_min(Yd)<<":"<<fct_max(Yd)<<"]\n";

  gp << "plot '-' with points pt 7 ps 0.5 lc rgb 'black' title 'I_{raw}'";
  gp << ", '-' with lines lc rgb 'red' title 'I_{cor}' \n";
  gp.send1d(std::tuple(Xd, Yd));
  gp.send1d(std::tuple(Xd, Xd));

  //---------------------------
  this->Clear();
}

//Regression
void Plotting::plot_Regression(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  if(keyOutside == true) gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' "+ format_data1 +", ";
  gp << "'-' "+ format_data2 +"\n";
  gp.send1d(std::tuple(Xd, Y1d));
  gp.send1d(std::tuple(Xd, Y2d));

  //---------------------------
  this->Clear();
}
void Plotting::plot_2Regression(vector<float>& Xd, vector<float>& Y1d1, vector<float>& Y2d1,vector<float>& Y1d2, vector<float>& Y2d2){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";

  auto minY=0, maxY=1;
  if(fct_min(Y1d1) < fct_min(Y2d1)) minY = fct_min(Y1d1); else minY = fct_min(Y2d1);
  if(fct_max(Y1d1) > fct_max(Y2d1)) maxY = fct_max(Y1d1); else maxY = fct_max(Y2d1);
  gp << "set xrange ["<<fct_min(Xd)<<":"<<fct_max(Xd)<<"]\n";
  gp << "set yrange ["<<minY<<":"<<maxY<<"]\n";

  gp << "plot '-' "+ dataFormA[0];
  gp << ", '-' "+ dataFormA[1];
  gp << ", '-' "+ dataFormA[2];
  gp << ", '-' "+ dataFormA[3] +"\n";
  gp.send1d(std::tuple(Xd, Y1d1));
  gp.send1d(std::tuple(Xd, Y2d1));
  gp.send1d(std::tuple(Xd, Y1d2));
  gp.send1d(std::tuple(Xd, Y2d2));

  //---------------------------
  this->Clear();
}
void Plotting::plot_MultipleCurveRegression(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& Y3d, vector<float>& Y4d, vector<float>& regr){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  //if(keyOutside == true) gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' "+ dataFormA[0] +", ";
  gp << "'-' "+ dataFormA[1] +", ";
  gp << "'-' "+ dataFormA[2] +", ";
  gp << "'-' "+ dataFormA[3] +", ";
  gp << "'-' "+ format_data2 +"\n";
  gp.send1d(std::tuple(Xd, Y1d));
  gp.send1d(std::tuple(Xd, Y2d));
  gp.send1d(std::tuple(Xd, Y3d));
  gp.send1d(std::tuple(Xd, Y4d));
  gp.send1d(std::tuple(Xd, regr));

  //---------------------------
  this->Clear();
}
void Plotting::plot_MultipleCurveRegression_tot(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& Y3d, vector<float>& Y4d, vector<float>& Reg1, vector<float>& Reg2, vector<float>& Reg3, vector<float>& Reg4){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  if(keyOutside == true) gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  gp << "plot '-' "+ dataFormA[0] +", ";
  gp << "'-' "+ dataFormA[1] +", ";
  gp << "'-' "+ dataFormA[2] +", ";
  gp << "'-' "+ dataFormA[3] +", ";
  gp << "'-' "+ format_data2 +", ";
  gp << "'-' "+ format_data2 +", ";
  gp << "'-' "+ format_data2 +", ";
  gp << "'-' "+ format_data2 +"\n";
  gp.send1d(std::tuple(Xd, Y1d));
  gp.send1d(std::tuple(Xd, Y2d));
  gp.send1d(std::tuple(Xd, Y3d));
  gp.send1d(std::tuple(Xd, Y4d));
  gp.send1d(std::tuple(Xd, Reg1));
  gp.send1d(std::tuple(Xd, Reg2));
  gp.send1d(std::tuple(Xd, Reg3));
  gp.send1d(std::tuple(Xd, Reg4));

  //---------------------------
  this->Clear();
}
void Plotting::plot_CurveErrorRegression(vector<float>& Xd, vector<float>& Y1d, vector<float>& Y2d, vector<float>& err){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange ["<<fct_min(Xd)<<":"<<fct_max(Xd)<<"]\n";
  gp << "set yrange ["<<fct_min(Y1d)<<":"<<fct_max(Y1d)<<"]\n";

  gp << "plot '-' with yerrorbars pt 7 ps 1 title '(I_{raw}-I_{min})d^2', ";
  gp << "'-' with lines lc rgb 'red' title 'Linear regression'\n";
  gp.send1d(std::tuple(Xd, Y1d, err));
  gp.send1d(std::tuple(Xd, Y2d));

  //---------------------------
  this->Clear();
}

//Histograms
void Plotting::plot_histogram(vector<float>& Xd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange ["<<fct_min(Xd)<<":"<<fct_max(Xd)<<"]\n";
  gp << "set yrange [0:]\n";
  //---------------------------

  //Histogram stuff
  gp << "binwidth=0.005\n";
  gp << "set style fill transparent solid 0.5 border -1\n";
  gp << "set boxwidth binwidth absolute\n";
  gp << "set bars fullwidth\n";
  gp << "bin(x,width)=width*floor(x/width)\n";

  gp << "plot '-' using (bin($1,binwidth)):(1) smooth frequency with boxes lc rgb'black' notitle";
  gp << ", '-' using (bin($1,binwidth)):(1) smooth frequency with boxes lc rgb'#77779fa1' notitle\n";
  gp.send1d(Xd);
  gp.send1d(Xd);

  //---------------------------
  this->Clear();
}
void Plotting::plot_2histograms(vector<float>& X1d, vector<float>& X2d){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set tics font ', "+ ticSize +"'\n";
  if(labelOn == false) gp << "set xlabel\n";
  if(labelOn == false) gp << "set ylabel\n";
  gp << "set xrange [0:1]\n";
  gp << "set yrange [0:*]\n";

  //Histogram stuff
  gp << "binwidth=0.005\n";
  gp << "set style fill transparent solid 0.5 border -1\n";
  gp << "set boxwidth binwidth absolute\n";
  gp << "set bars fullwidth\n";
  gp << "bin(x,width)=width*floor(x/width)\n";

  gp << "plot '-' using (bin($1,binwidth)):(1) smooth frequency with boxes lc rgb'#77779fa1' title 'I_{0}',"
  << "'-' using (bin($1,binwidth)):(1) smooth frequency with boxes lc rgb'#77ff6542' title 'I_{c}'\n";
  gp.send1d(X1d);
  gp.send1d(X2d);

  //---------------------------
  this->Clear();
}

//Specifics
void Plotting::curve_Spectralon(){
  Gnuplot gp("tee '../graphs/gnuplot/DATA_Spectralon.gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n";
  gp << "set key outside\n";
  if(keyOn == false) gp << "unset key\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n";
  gp << "set xlabel 'Wavelength (nm)'\n";
  gp << "set ylabel 'Reflectance factor'\n";
  gp << "set xrange [0:2750]\n";
  gp << "set yrange [0:1]\n";

  string line;
  string pathFile = "../media/Spectralon_CalibrationData.txt";
  std::ifstream infile(pathFile);
  float d_x, d_99, d_50, d_25, d_10;
  vector<float> v_x, v_99, v_50, v_25, v_10;

  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> d_x >> d_99 >> d_50 >> d_25 >> d_10;

    v_x.push_back(d_x);
    v_99.push_back(d_99);
    v_50.push_back(d_50);
    v_25.push_back(d_25);
    v_10.push_back(d_10);
  }

  string a1(color_a1), a2(color_a2), a3(color_a3), a4(color_a4);
  gp << "plot '-' with lines lc rgb '"+ a1 +"' title '99%'";
  gp << ", '-' with lines lc rgb '"+ a2 +"' title '50%'";
  gp << ", '-' with lines lc rgb '"+ a3 +"' title '25%'";
  gp << ", '-' with lines lc rgb '"+ a4 +"' title '10%'\n";
  gp.send1d(std::tuple(v_x, v_99));
  gp.send1d(std::tuple(v_x, v_50));
  gp.send1d(std::tuple(v_x, v_25));
  gp.send1d(std::tuple(v_x, v_10));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3Dmap_vec3(vector<vec3> vec_in){
  Gnuplot gp;
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  vector<float> X, Y, Z;
  for(int i=0; i<vec_in.size(); i++){
    X.push_back(vec_in[i][2]);
    Y.push_back(vec_in[i][1]);
    Z.push_back(vec_in[i][0]);
  }

  gp << "splot '-' with points pt 7 ps 0.5 lc rgb 'black' notitle\n";
  gp.send1d(std::tuple(X, Y, Z));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3Dmap(vector<vector<float>>& Xd, vector<vector<float>>& Yd, vector<vector<float>>& Zd){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set colorbox\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";

  gp << "splot ";
  for(int i=0; i<Xd.size(); i++){
    if(i != 0) gp << ", ";
    gp << "'-' with linespoints pt 7 ps 0.5 palette notitle";
  }
  gp << "\n";
  for(int i=0; i<Xd.size(); i++){
    gp.send1d(std::tuple(Xd[i], Yd[i], Zd[i]));
  }

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapRegression(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su){
  Gnuplot gp;
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  gp << "set style fill transparent solid 0.60\n";
  gp << "set hidden3d\n";
  gp << "set pm3d\n";

  //Surface
  gp << "set table 'data.dat'\n";
  gp << "set dgrid3d 50,50 splines\n";
  gp << "splot '-'\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_su));
  gp << "unset dgrid3d\n";
  gp << "unset table\n";

  //Points
  gp << "splot '-' with points pt 7 ps 0.5 lc rgb 'black' notitle";
  gp << ", '-' with points pt 7 ps 1 lc rgb 'red' notitle";
  gp << ", 'data.dat' with points pt 14 ps 0.0001 lc palette notitle\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_pt));
  gp.send1d(std::tuple(Xd, Yd, Zd_su));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapRegression_title(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su, string t1){
  Gnuplot gp;
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  gp << "set style fill transparent solid 0.60\n";
  gp << "set hidden3d\n";
  gp << "set pm3d\n";

  //Surface
  gp << "set table 'data.dat'\n";
  gp << "set dgrid3d 50,50 splines\n";
  gp << "splot '-'\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_su));
  gp << "unset dgrid3d\n";
  gp << "unset table\n";

  //Points
  gp << "splot '-' with points pt 7 ps 0.5 lc rgb 'black' title '"+t1+"'";
  gp << ", '-' with points pt 7 ps 1 lc rgb 'red' notitle";
  gp << ", 'data.dat' with points pt 14 ps 0.0001 lc palette notitle\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_pt));
  gp.send1d(std::tuple(Xd, Yd, Zd_su));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapRegression_points(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt, vector<float>& Zd_su){
  Gnuplot gp;
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid x y z vertical lt 1 lw 0.1 lc rgb 'black'\n";
  gp << "set hidden3d\n";

  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  float zmean = fct_mean(Zd_pt);
  float xmin = fct_min(Xd);
  float xmax = fct_max(Xd);
  float ymin = fct_min(Yd);
  float ymax = fct_max(Yd);

  ostringstream zmean_o, xmin_o, xmax_o, ymin_o, ymax_o;
  zmean_o << zmean;
  xmin_o << xmin;  xmax_o << xmax;
  ymin_o << ymin;  ymax_o << ymax;

  string zmean_s(zmean_o.str());
  string xmin_s(xmin_o.str());
  string xmax_s(xmax_o.str());
  string ymin_s(ymin_o.str());
  string ymax_s(ymax_o.str());

  gp << "set arrow from "+ xmin_s +", "+ ymin_s +", "+ zmean_s +" to "+ xmax_s +", "+ ymin_s +", "+ zmean_s +" lt 2 lw 0.5 lc rgb 'black' nohead\n";
  gp << "set arrow from "+ xmin_s +", "+ ymax_s +", "+ zmean_s +" to "+ xmax_s +", "+ ymax_s +", "+ zmean_s +" lt 2 lw 0.5 lc rgb 'black' nohead\n";
  gp << "set arrow from "+ xmin_s +", "+ ymin_s +", "+ zmean_s +" to "+ xmin_s +", "+ ymax_s +", "+ zmean_s +" lt 2 lw 0.5 lc rgb 'black' nohead\n";
  gp << "set arrow from "+ xmax_s +", "+ ymin_s +", "+ zmean_s +" to "+ xmax_s +", "+ ymax_s +", "+ zmean_s +" lt 2 lw 0.5 lc rgb 'black' nohead\n";

  //Points
  gp << "splot '-' with points pt 7 ps 0.5 lc rgb 'black' notitle";
  gp << ", '-' with points pt 7 ps 1 lc rgb 'red' notitle\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_pt));
  gp.send1d(std::tuple(Xd, Yd, Zd_su));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapSpaceParameter(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_pt){
  Gnuplot gp;
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  //Points
  gp << "splot '-' with points pt 7 ps 0.5 lc rgb 'black' notitle\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_pt));

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapRegression_mat(MatrixXf& mat){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  //Create file data
  ofstream myfile;
  myfile.open ("data.txt");

  for(int i=0; i<mat.rows(); i++){
    for(int j=0; j<mat.cols(); j++){
      if(j != mat.cols()-1){
        myfile << mat(i,j) << ", ";
      }
      else{
        myfile << mat(i,j);
      }
    }
    myfile << "\n";
  }
  myfile.close();

  gp << "set grid\n";
  gp << "set hidden3d\n";

  gp << "splot 'data.txt' matrix with lines notitle\n";

  //---------------------------
  this->Clear();
}
void Plotting::plot_3DmapOnlyRegression(vector<float>& Xd, vector<float>& Yd, vector<float>& Zd_su){
  Gnuplot gp("tee '../media/data/graphs/" + namePlot + ".gp' | gnuplot -persist");
  //---------------------------

  gp << "set terminal wxt\n";
  gp << "set grid\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";

  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set zlabel '" + Zlabel + "'\n";
  gp << "set zlabel rotate\n";

  gp << "set tics font ', "+ ticSize +"'\n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";
  gp << "set zrange "+ zRange +"\n";
  gp << "set mouse\n";

  gp << "set style fill transparent solid 0.60\n";
  gp << "set hidden3d\n";
  gp << "set pm3d\n";

  gp << "splot '-' "+ format_data1 +"\n";
  gp.send1d(std::tuple(Xd, Yd, Zd_su));

  //---------------------------
  this->Clear();
}
void Plotting::plot_2Dmap(MatrixXf HM, vector<float>& R_map, vector<float>& cosIt_map){
  Gnuplot gp;
  //---------------------------

  gp << "set grid\n";
  gp << "unset key\n";
  gp << "set style increment default\n";
  gp << "set xlabel '" + Xlabel + "'\n";
  gp << "set ylabel '" + Ylabel + "'\n";
  gp << "set colorbox\n";
  gp << "set view map\n";
  gp << "set palette \n";
  gp << "set xrange "+ xRange +"\n";
  gp << "set yrange "+ yRange +"\n";

  //Create file data
  ofstream myfile;
  myfile.open ("data.txt");

  for(int i=0; i<HM.rows(); i++){
    for(int j=0; j<HM.cols(); j++){
      if(j != HM.cols()-1){
        myfile << HM(i,j) << ", ";
      }
      else{
        myfile << HM(i,j);
      }
    }
    myfile << "\n";
  }
  myfile.close();

  gp << "splot 'data.txt' matrix with image notitle\n";

  //---------------------------
}
void Plotting::plot_PointCloud(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), 0);
  Gnuplot gp;
  //---------------------------

  gp << "set grid\n"; //Grid
  gp << "set key opaque box lt -1 lw 2\n";
  gp << "set style line 11 lc rgb '#606060' lt 1\n";
  gp << "set border 3 back ls 11\n";
  gp << "set tics nomirror\n"; //Supress top and rigth tics
  gp << "set xlabel 'X'\n";
  gp << "set ylabel 'Y'\n";
  gp << "set zlabel 'Z'\n";
  gp << "set style function linespoints\n";
  gp << "set size square\n";
  gp << "set yrange [0:1]\n";
  gp << "set zrange [0:1]\n";

  vector<float>& I = subset->I;
  vector<vec3>& XYZ = subset->xyz;
  vector<float> X, Y, Z;
  for(int i=0; i<XYZ.size(); i++){
    X.push_back(XYZ[i].x);
    Y.push_back(XYZ[i].y);
    Z.push_back(XYZ[i].z);
  }
  gp << "set xrange ["<<fct_min(X)<<":"<<fct_max(X)<<"]\n";

  gp << "splot '-' using 1:2:3:4 with points ps 1 pt 7 palette notitle\n";
  gp.send1d(std::tuple(X, Y, Z, I));

  //---------------------------
}
