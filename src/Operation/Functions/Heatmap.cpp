#include "Heatmap.h"

#include "../Transformation/Attribut.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Data/Database.h"
#include "../../Specific/fct_maths.h"

extern struct Database database;



//Constructor / destructor
Heatmap::Heatmap(){
  //---------------------------

  this->sceneManager = new Scene();
  this->attribManager = new Attribut();

  this->HMmode = 0;
  this->normalized = true;
  this->range_norm = vec2(0.0f, 1.0f);
  this->range_height = vec2(-2.0f, 2.0f);

  //---------------------------
}
Heatmap::~Heatmap(){}

//HMI functions
void Heatmap::set_Heatmap(Cloud* cloud){
  bool& is_heatmap = cloud->heatmap;
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];

    //Apply heatmap
    if(is_heatmap == false){
      this->compute_subset_heatmap_ON(subset);
    }

    //Reverse heatmap
    if(is_heatmap == true){
      this->compute_subset_heatmap_OFF(subset);
    }
  }

  //Inverse old boolean
  is_heatmap = !is_heatmap;

  //---------------------------
  sceneManager->update_cloud_color(cloud);
}
void Heatmap::set_Heatmap(Cloud* cloud, bool is_heatmap){
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];

    //Apply heatmap
    if(is_heatmap == true){
      this->compute_subset_heatmap_ON(subset);
      cloud->subset_buffer[i].RGB = subset->RGB;
    }

    //Reverse heatmap
    if(is_heatmap == false){
      this->compute_subset_heatmap_OFF(subset);
      cloud->subset_buffer[i].RGB = subset->RGB;
    }
  }

  //---------------------------
  sceneManager->update_cloud_color(cloud);
}
void Heatmap::set_Heatmap_all(bool heatAll){
  list<Cloud*>* list = database.list_cloud;
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);

    cloud->heatmap = heatAll;
    this->set_Heatmap(cloud);
  }

  //---------------------------
}

//Processing functions
void Heatmap::compute_subset_heatmap_ON(Subset* subset){
  //---------------------------

  //Apply heatmap
  switch(HMmode){
    case 0:{//height
      vector<float> z_vec = attribManager->get_z_vector(subset->xyz);
      //TODO: normalize by z min max of the overall cloud

      //Check for preventing too much near range
      if(range_height.x + 1 > range_height.y){
        range_height.x = range_height.y - 1;
      }

      vector<float> z_vec_norm = fct_normalize(z_vec, range_height);
      vector<float>& z_vec_n = z_vec_norm;
      this->compute_heatmap_color(subset, z_vec_n);
      break;
    }
    case 1:{//Is
      vector<float>& Is = subset->I;
      if(Is.size() != 0){
        vector<float> data;
        for(int i=0; i<Is.size(); i++){
          if(Is[i] >= range_norm.x && Is[i] <= range_norm.y){
            //If in range
            data.push_back(Is[i]);
          }else{
            //If out of range
            data.push_back(-1);
          }
        }
        this->compute_heatmap_color(subset, data);
      }
      break;
    }
    case 2:{//dist
      vector<float>& dist = subset->R;
      if(dist.size() == 0) attribManager->compute_subset_distance(subset);
      vector<float> dist_norm = fct_normalize(dist);
      vector<float>& dist_n = dist_norm;
      this->compute_heatmap_color(subset, dist_n);
      break;
    }
    case 3:{//cosIt
      vector<float>& cosIt = subset->cosIt;
      if(cosIt.size() == 0) attribManager->compute_subset_cosIt(subset);
      this->compute_heatmap_color(subset, cosIt);
      break;
    }
    case 4:{//It
      vector<float>& It = subset->It;
      if(It.size() == 0) attribManager->compute_subset_cosIt(subset);
      vector<float> It_norm = fct_normalize(It);
      vector<float>& It_n = It_norm;
      this->compute_heatmap_color(subset, It_n);
      break;
    }
  }

  //---------------------------
}
void Heatmap::compute_subset_heatmap_OFF(Subset* subset){
  int size = subset->nb_point;
  vector<vec4>& RGB = subset->RGB;
  vector<float>& Is = subset->I;
  //---------------------------

  //If intensity, reapply color
  if(Is.size() != 0){
    for(int i=0; i<size; i++){
      RGB[i] = vec4(Is[i],Is[i],Is[i],1);
    }
  }
  //else reapply random color
  else{
    for(int i=0; i<size; i++){
      RGB[i] = subset->unicolor;
    }
  }


  //---------------------------
}
void Heatmap::compute_heatmap_color(Subset* subset, vector<float>& v_in){
  vector<vec4>& RGB = subset->RGB;
  const int NUM_COLORS = 4;
  static float color[NUM_COLORS][3] = { {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0} };
  //---------------------------

  int idx1;        // |-- Our desired color will be between these two indexes in "color".
  int idx2;        // |
  float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.
  float red, green, blue, alpha;

  vector<float> v_norm;
  if(normalized){
    v_norm = fct_normalize(v_in, range_norm);
  }else{
    v_norm = v_in;
  }

  for(int i=0; i<RGB.size(); i++){
    if(v_in[i] != -1 && isnan(v_norm[i]) == false){
      float value = v_norm[i];
      value = value * (NUM_COLORS-1);        // Will multiply value by 3.
      idx1  = floor(value);                  // Our desired color will be after this index.
      idx2  = idx1+1;                        // ... and before this index (inclusive).
      fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).

      red   = (color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0];
      green = (color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1];
      blue  = (color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2];

      RGB[i] = vec4(red, green, blue, 1.0f);
    }
    else{
      //RGB[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
      RGB[i] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
  }

  //---------------------------
}

//Plotting
void Heatmap::plot_colorPalette(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
  vector<float> v_in;
  //---------------------------

  switch(HMmode){
    case 0:{ //Is
      v_in = subset->I;
      break;
    }
    case 1:{ //dist
      v_in = subset->R;
      break;
    }
    case 2:{ //cosIt
      v_in = subset->cosIt;
      break;
    }
    case 3:{ //It
      v_in = subset->It;
      break;
    }
  }

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
}
