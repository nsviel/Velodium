#include "Light.h"

#include "../GPU/GPU_data.h"
#include "../Node_engine.h"

#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Pose.h"


//Constructor / Destructor
Light::Light(){
  //---------------------------

  this->loaderManager = new Loader();
  this->sceneManager = new Scene();
  this->transformManager = new Transformation();
  this->poseManager = new Pose();
  this->gpuManager = new GPU_data();
  this->data = Data::get_instance();

  //---------------------------
}
Light::~Light(){}

void Light::init(){
  std::list<Light_*>* list_light = data->get_list_light();
  //---------------------------

  Light_* light = (Light_*)loaderManager->load_object("../media/engine/Marks/sphere.obj");

  light->name = "Light";
  light->is_visible = true;
  transformManager->make_scaling(light, 0.1);
  gpuManager->update_buffer_location(light);
  gpuManager->update_buffer_color(light);
  sceneManager->update_MinMax(light);
  list_light->push_back(light);

  //---------------------------
}
