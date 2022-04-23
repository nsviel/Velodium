#include "Colormap.h"

#include "../../Specific/fct_maths.h"


//Constructor / destructor
Colormap::Colormap(){
  //---------------------------


  //---------------------------
  this->init();
}
Colormap::~Colormap(){}

void Colormap::init(){
  //---------------------------

  //Colormap - Viridis
  vector<vec3> viridis = {vec3(0.267004, 0.004874, 0.329415), vec3(0.252194, 0.269783, 0.531579),
                          vec3(0.180629, 0.429975, 0.557282), vec3(0.133743, 0.548535, 0.553541),
                          vec3(0.137339, 0.662252, 0.515571), vec3(0.304148, 0.764704, 0.419943),
                          vec3(0.575563, 0.844566, 0.256415), vec3(0.993248, 0.906157, 0.143936)};
  this->colormap_name.push_back("viridis");
  this->colormap_list.push_back(viridis);

  //Colormap - Magma
  vector<vec3> magma = {vec3(0.001462, 0.000466, 0.013866), vec3(0.043830, 0.033830, 0.141886),
                          vec3(0.232077, 0.059889, 0.437695), vec3(0.439062, 0.120298, 0.506555),
                          vec3(0.620005, 0.183840, 0.497524), vec3(0.804752, 0.249911, 0.442102),
                          vec3(0.953099, 0.397563, 0.361438), vec3(0.993326, 0.602275, 0.414390),
                          vec3(0.995131, 0.827052, 0.585701), vec3(0.987053, 0.991438, 0.749504)};
  this->colormap_name.push_back("magma");
  this->colormap_list.push_back(magma);

  //Colormap - Rainbow
  vector<vec3> rainbow = {vec3(0,0,1), vec3(0,1,0), vec3(1,1,0), vec3(1,0,0)};
  this->colormap_name.push_back("rainbow");
  this->colormap_list.push_back(rainbow);

  //Selected colormap
  this->colormap_selected = viridis;

  //---------------------------
}
void Colormap::choose(string name){
  //---------------------------

  for(int i=0; i<colormap_name.size(); i++){
    if(name == colormap_name[i]){
      colormap_selected = colormap_list[i];
    }
  }


  //---------------------------
}
void Colormap::plot(vector<float> v_in){/*
  //---------------------------

  //Pre-processing
  vector<float> v_Is;
  vector<vec4> v_RGB;
  for(int i=0; i<v_in.size(); i++){
    if(v_in[i] != -1){
      v_Is.push_back(v_in[i]);
      v_RGB.push_back(RGB[i]);
    }
  }

  vector<size_t> idx = fct_sortByIndexes(v_Is);
  sort(v_Is.begin(), v_Is.end());

  //Plotting
  bool flag = true;
  for(int i=0; i<2; i++){
    Gnuplot gp("tee '../graphs/gnuplot/PaletteColor.gp' | gnuplot -persist");

    if(flag){
      gp << "set terminal pngcairo\n";
      gp << "set output '../graphs/PaletteColor.png'\n";
      flag = false;
    }

    //Style
    gp << "g(y)=y\n";
    gp << "set format y '%-.2f'\n";
    gp << "unset key\n";

    gp << "set view map\n";
    gp << "set style data pm3d\n";
    gp << "set style function pm3d\n";
    gp << "set palette rgb 33,13,10\n";

    //gp << "set size ratio 2\n";
    gp << "set colorbox size 5,20\n";

    gp << "unset xtics\n";
    gp << "set ytics "<< fct_min(v_Is) <<","<< (fct_max(v_Is)-fct_min(v_Is))/10 <<","<< fct_max(v_Is) <<" scale 1.5 nomirror\n";
    gp << "unset ztics\n";
    gp << "set yrange ["<< fct_min(v_Is) <<":"<< fct_max(v_Is) <<"]\n";
    gp << "set mytics 2\n";
    //---------------------------------

    //Plot palette color
    gp << "set palette defined (";
    for(int i=0; i<v_Is.size(); i=i+v_Is.size()/1000){
      // #num R G B
      gp <<v_Is[i]<<" "<<v_RGB[idx[i]][0]<<" "<<v_RGB[idx[i]][1]<<" "<<v_RGB[idx[i]][2];
      if((i+v_Is.size()/1000) < v_Is.size()){
        gp << ", ";
      }else{
        gp << ")\n";
      }
    }

    gp << "splot g(y)\n";
  }


  //---------------------------
*/}
