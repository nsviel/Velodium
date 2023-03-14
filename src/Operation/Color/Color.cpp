#include "Color.h"

#include "Heatmap.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/GPU/GPU_data.h"
#include "../../Scene/Node_scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Specific/Function/fct_math.h"

#include <random>

/* color_mode
 * 0 = unicolor
 * 1 = intensity
 * 2 = heatmap
*/


glm::vec4 random_color(){
  //---------------------------

  float Red = float(rand()%101)/100;
  float Green = float(rand()%101)/100;
  float Blue = float(rand()%101)/100;
  glm::vec4 color = glm::vec4(Red, Green, Blue, 1.0f);

  //---------------------------
  return color;
}


//Constructor / destructor
Color::Color(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->gpuManager = new GPU_data();

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

//Color cloud functions
void Color::make_colorization(Collection* collection, int ID_object){
  Object_* object = collection->get_obj_byID(ID_object);
  //---------------------------

  switch(color_mode){
    case 0:{ // Unicolor
      this->color_unicolor(object, collection->unicolor);
      break;
    }
    case 1:{ // Intensity
      if(object->obj_type == "cloud")
        this->color_intensity((Cloud*)object);
      break;
    }
    case 2:{ // Heatmap
      if(object->obj_type == "cloud")
        this->color_heatmap((Cloud*)object);
      break;
    }
  }

  //---------------------------
}
void Color::make_colorization(Cloud* cloud, vec4 RGB_in){
  vector<vec4>& RGB = cloud->rgb;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = RGB_in;
  }

  //---------------------------
  gpuManager->update_buffer_color(cloud);
}
void Color::make_colorization_specific(Cloud* cloud){
  vector<vec4>& RGB = cloud->rgb;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = specific_color;
  }

  //---------------------------
  gpuManager->update_buffer_color(cloud);
}

void Color::color_unicolor(Object_* object, vec4 color){
  vector<vec4>& RGB = object->rgb;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = color;
  }

  //---------------------------
  gpuManager->update_buffer_color(object);
}
void Color::color_intensity(Cloud* cloud){
  vector<vec4>& RGB = cloud->rgb;
  vector<float>& Is = cloud->I;
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
  gpuManager->update_buffer_color(cloud);
}
void Color::color_heatmap(Cloud* cloud){
  //---------------------------

  heatmapManager->make_cloud_heatmap(cloud);

  //---------------------------
}

//Color cloud functions
void Color::set_color_new(Collection* collection, vec4 RGBA){
  collection->unicolor = RGBA;
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    vector<vec4>& RGB = cloud->rgb;
    vector<float>& Is = cloud->I;
    //---------------------------

    for(int i=0; i<RGB.size(); i++){
      RGB[i] = vec4(RGBA.x , RGBA.y, RGBA.z, RGBA.w);
    }

    //---------------------------
    gpuManager->update_buffer_color(cloud);
  }
}
void Color::set_color_RGB(Collection* collection){
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    vector<vec4>& RGB_obj = cloud->rgb;
    vector<vec4>& RGB_ini = cloud->rgb;
    //---------------------------

    RGB_obj = RGB_ini;

    //---------------------------
    gpuManager->update_buffer_color(cloud);
  }
}
void Color::set_color_I(Collection* collection){
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    vector<vec4>& RGB_obj = cloud->rgb;
    vector<float>& Is = cloud->I;
    //---------------------------

    RGB_obj.clear();
    for(int i=0; i<Is.size(); i++){
      RGB_obj.push_back(vec4(Is[i], Is[i], Is[i], 1.0f));
    }

    //---------------------------
    gpuManager->update_buffer_color(cloud);
  }
}
void Color::set_color_enhanced(Collection* collection){
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    vector<float>& Is = cloud->I;
    const vector<vec4>& RGB = cloud->rgb;
    //---------------------------

    for(int i=0; i<RGB.size(); i++){
      vec4 rgb = RGB[i];
      float& I = Is[i];
      rgb = vec4(rgb.x*I, rgb.y*I, rgb.z*I, 1.0f);
    }

    //---------------------------
    gpuManager->update_buffer_color(cloud);
  }
}
void Color::set_color_random(Collection* collection){
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    vector<vec4>& RGB = cloud->rgb;
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
void Color::set_color_initial(Collection* collection){
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    Cloud* list_obj_init = (Cloud*)*next(collection->list_obj_init.begin(), i);
    //---------------------------

    cloud->rgb = list_obj_init->rgb;

    //---------------------------
    gpuManager->update_buffer_color(cloud);
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
