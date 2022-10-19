#include "Color.h"

#include "Heatmap.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_maths.h"

/* color_mode
 * 0 = unicolor
 * 1 = intensity
 * 2 = heatmap
*/


//Constructor / destructor
Color::Color(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->heatmapManager = node_ope->get_heatmapManager();

  //---------------------------
  this->update_configuration();
}
Color::~Color(){}

void Color::update_configuration(){
  //---------------------------

  int min = configManager->parse_json_i("parameter", "color_intensity_min");
  int max = configManager->parse_json_i("parameter", "color_intensity_max");
  vec2* heatmap_I_range = heatmapManager->get_range_intensity();
  vec2 intensity_range = vec2((float) min/255, (float) max/255);

  *heatmap_I_range = intensity_range;
  this->color_mode = configManager->parse_json_i("parameter", "color_mode");
  this->range_intensity = intensity_range;
  this->specific_color = vec4(1, 1, 1, 1);

  //---------------------------
}

//Color subset functions
void Color::make_colorization(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  switch(color_mode){
    case 0:{ // Unicolor
      this->color_unicolor(subset, cloud->unicolor);
      break;
    }
    case 1:{ // Intensity
      this->color_intensity(subset);
      break;
    }
    case 2:{ // Heatmap
      this->color_heatmap(subset);
      break;
    }
  }

  //---------------------------
}
void Color::make_colorization(Subset* subset, vec4 RGB_in){
  vector<vec4>& RGB = subset->RGB;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = RGB_in;
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}
void Color::make_colorization_specific(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = specific_color;
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}

void Color::color_unicolor(Subset* subset, vec4 color){
  vector<vec4>& RGB = subset->RGB;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = color;
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}
void Color::color_intensity(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
  vector<float>& Is = subset->I;
  //---------------------------

  if(Is.size() != 0){
    //fct_normalize intensity wrt range
    vector<float> In;
    for(int i=0; i<Is.size(); i++){
      if(Is[i] < range_intensity.x || Is[i] > range_intensity.y){
        In.push_back(-1);
      }else{
        In.push_back(Is[i]);
      }
    }
    In = fct_normalize(In, -1);

    //Apply intensity and discard for not-in-range intensities
    for(int i=0; i<RGB.size(); i++){
      if(In[i] == -1){
        RGB[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
      }else{
        RGB[i] = vec4(In[i], In[i], In[i], 1.0f);
      }
    }
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}
void Color::color_heatmap(Subset* subset){
  //---------------------------

  heatmapManager->make_subset_heatmap(subset);

  //---------------------------
}

//Color cloud functions
void Color::set_color_new(Cloud* cloud, vec4 RGBA){
  cloud->unicolor = RGBA;
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    vector<vec4>& RGB = subset->RGB;
    vector<float>& Is = subset->I;
    //---------------------------

    for(int i=0; i<RGB.size(); i++){
      RGB[i] = vec4(RGBA.x , RGBA.y, RGBA.z, RGBA.w);
    }

    //---------------------------
    sceneManager->update_subset_color(subset);
  }
}
void Color::set_color_RGB(Cloud* cloud){
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    vector<vec4>& RGB_obj = subset->RGB;
    vector<vec4>& RGB_ini = subset->RGB;
    //---------------------------

    RGB_obj = RGB_ini;

    //---------------------------
    sceneManager->update_subset_color(subset);
  }
}
void Color::set_color_I(Cloud* cloud){
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    vector<vec4>& RGB_obj = subset->RGB;
    vector<float>& Is = subset->I;
    //---------------------------

    RGB_obj.clear();
    for(int i=0; i<Is.size(); i++){
      RGB_obj.push_back(vec4(Is[i], Is[i], Is[i], 1.0f));
    }

    //---------------------------
    sceneManager->update_subset_color(subset);
  }
}
void Color::set_color_enhanced(Cloud* cloud){
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    vector<float>& Is = subset->I;
    const vector<vec4>& RGB = subset->RGB;
    //---------------------------

    for(int i=0; i<RGB.size(); i++){
      vec4 rgb = RGB[i];
      float& I = Is[i];
      rgb = vec4(rgb.x*I, rgb.y*I, rgb.z*I, 1.0f);
    }

    //---------------------------
    sceneManager->update_subset_color(subset);
  }
}
void Color::set_color_random(Cloud* cloud){
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    vector<vec4>& RGB = subset->RGB;
    //---------------------------

    float Red, Green, Blue;
    for(int i=0; i<RGB.size(); i++){
      Red = float(rand()%101)/100;
      Green = float(rand()%101)/100;
      Blue = float(rand()%101)/100;

      RGB[i] = vec4(Red, Green, Blue, 1.0f);
    }

    //---------------------------
  }
}
void Color::set_color_initial(Cloud* cloud){
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    Subset* subset_init = *next(cloud->subset_init.begin(), i);
    //---------------------------

    subset->RGB = subset_init->RGB;

    //---------------------------
    sceneManager->update_subset_color(subset);
  }
}
string Color::get_color_mode_name(){
  string name;
  //---------------------------

  switch(color_mode){
    case 0:{ // Unicolor
      name = "Unicolor";
      break;
    }
    case 1:{ // Intensity
      name = "Intensity";
      break;
    }
    case 2:{ // Heatmap
      name = "Heatmap";
      break;
    }
  }

  //---------------------------
  return name;
}
