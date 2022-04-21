#include "Heatmap.h"

#include "../Operation_node.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_maths.h"

/* heatmap_mode
 * 0 = height
 * 1 = intensity
 * 2 = distance
 * 3 = incidence angle cosine
 * 4 = incidence angle
*/


//Constructor / destructor
Heatmap::Heatmap(Operation_node* node_ope){
  //---------------------------

  Engine_node* node_engine = node_ope->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();

  this->heatmap_mode = 1;
  this->is_normalization = true;
  this->range_norm = vec2(0.0f, 1.0f);
  this->range_height = vec2(-2.0f, 2.0f);
  this->range_intensity = vec2(0.0f, 1.0f);

  //---------------------------
  this->define_colormap();
}
Heatmap::~Heatmap(){}

//HMI functions
void Heatmap::make_heatmap_all(bool is_heatmap){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    cloud->heatmap = is_heatmap;
    this->make_cloud_heatmap(cloud);
  }

  //---------------------------
}
void Heatmap::make_cloud_heatmap(Cloud* cloud){
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Subset* subset_buf = sceneManager->get_subset_buffer(cloud, i);
    Subset* subset_ini = sceneManager->get_subset_init(cloud, i);

    //Apply heatmap
    if(cloud->heatmap == false){
      subset_buf->RGB = subset->RGB;
      this->make_subset_heatmap(subset);
      cloud->heatmap = true;
    }
    //Reverse heatmap
    else{
      //this->heatmap_unset(subset);
      subset->RGB = subset_buf->RGB;
      cloud->heatmap = false;
    }
  }

  //---------------------------
  sceneManager->update_cloud_color(cloud);
}
void Heatmap::make_subset_heatmap(Subset* subset){
  //---------------------------

  switch(heatmap_mode){
    case 0:{// height
      this->mode_height(subset);
      break;
    }
    case 1:{// intensity
      this->mode_intensity(subset);
      break;
    }
    case 2:{// distance
      this->mode_distance(subset);
      break;
    }
    case 3:{// incidence angle cosine
      this->mode_cosIt(subset);
      break;
    }
    case 4:{// incidence angle
      this->mode_It(subset);
      break;
    }
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}

//Specific mode functions
void Heatmap::mode_height(Subset* subset){
  //---------------------------

  vector<float> z_vec = attribManager->get_z_vector(subset->xyz);

  //Check for preventing too much near range
  if(range_height.x + 1 > range_height.y){
    range_height.x = range_height.y - 1;
  }

  //Normalize resulting color vector
  vector<float> z_vec_norm = fct_normalize(z_vec, range_height);
  vector<float>& color_vec = z_vec_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_intensity(Subset* subset){
  if(subset->I.size() != 0){
    //---------------------------

    vector<float>& Is = subset->I;
    vector<float> color_vec;

    for(int i=0; i<Is.size(); i++){
      //If point i intensity is in range
      if(Is[i] >= range_intensity.x && Is[i] <= range_intensity.y){
        color_vec.push_back(Is[i]);
      }
      //If point i intensity is out of range
      else{
        color_vec.push_back(-1);
      }
    }

    //---------------------------
    this->heatmap_set(subset, color_vec);
  }
}
void Heatmap::mode_distance(Subset* subset){
  vector<float>& dist = subset->R;
  //---------------------------

  if(dist.size() == 0){
    attribManager->compute_subset_distance(subset);
  }

  vector<float> dist_norm = fct_normalize(dist);
  vector<float>& color_vec = dist_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_cosIt(Subset* subset){
  vector<float>& color_vec = subset->cosIt;
  //---------------------------

  if(color_vec.size() == 0){
    attribManager->compute_subset_cosIt(subset);
  }

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_It(Subset* subset){
  vector<float>& It = subset->It;
  //---------------------------

  if(It.size() == 0){
    attribManager->compute_subset_cosIt(subset);
  }

  vector<float> It_norm = fct_normalize(It);
  vector<float>& color_vec = It_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}

//Processing functions
void Heatmap::heatmap_set(Subset* subset, vector<float>& v_in){
  vector<vec4>& RGB = subset->RGB;
  //---------------------------

  //Normalization of the input vector
  vector<float> v_norm;
  if(is_normalization){
    v_norm = fct_normalize(v_in, -1);
  }else{
    v_norm = v_in;
  }

  //Compute heatmap from input vector
  for(int i=0; i<RGB.size(); i++){
    if(v_in[i] != -1 && isnan(v_norm[i]) == false){
      float value = v_norm[i] * (colormap_selected.size()-1);        // Will multiply value by 3.
      float idx1  = floor(value);                  // Our desired color will be after this index.
      float idx2  = idx1 + 1;                        // ... and before this index (inclusive).
      float fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).

      float red   = (colormap_selected[idx2][0] - colormap_selected[idx1][0]) * fractBetween + colormap_selected[idx1][0];
      float green = (colormap_selected[idx2][1] - colormap_selected[idx1][1]) * fractBetween + colormap_selected[idx1][1];
      float blue  = (colormap_selected[idx2][2] - colormap_selected[idx1][2]) * fractBetween + colormap_selected[idx1][2];

      RGB[i] = vec4(red, green, blue, 1.0f);
    }
    else{
      RGB[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }

  //---------------------------
}
void Heatmap::heatmap_unset(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
  vector<float>& Is = subset->I;
  //---------------------------

  //If intensity, reapply color
  if(Is.size() != 0){
    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i],1);
    }
  }
  //else reapply random color
  else{
    for(int i=0; i<RGB.size(); i++){
      RGB[i] = subset->unicolor;
    }
  }


  //---------------------------
}

//Plotting
void Heatmap::define_colormap(){
  //---------------------------

  vector<vec3> colormap_viridis = {vec3(0.267004, 0.004874, 0.329415), vec3(0.252194, 0.269783, 0.531579),
                          vec3(0.180629, 0.429975, 0.557282), vec3(0.133743, 0.548535, 0.553541),
                          vec3(0.137339, 0.662252, 0.515571), vec3(0.304148, 0.764704, 0.419943),
                          vec3(0.575563, 0.844566, 0.256415), vec3(0.993248, 0.906157, 0.143936)};
  vector<vec3> colormap_rainbow = {vec3(0,0,1), vec3(0,1,0), vec3(1,1,0), vec3(1,0,0)};
  
  this->colormap_selected = colormap_viridis;

  //---------------------------
}
void Heatmap::plot_colorPalette(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
  vector<float> v_in;
  //---------------------------

  switch(heatmap_mode){
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
