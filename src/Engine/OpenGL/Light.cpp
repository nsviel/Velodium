#include "Light.h"

#include "../Node_engine.h"

#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Pose.h"


//Constructor / Destructor
Light::Light(Node_engine* node){
  //---------------------------

  Node_load* node_load = node->get_node_load();
  Node_scene* node_scene = node->get_node_scene();

  this->loaderManager = node_load->get_loaderManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->transformManager = new Transformation();
  this->poseManager = new Pose();
  this->data = Data::get_instance();

  //---------------------------
}
Light::~Light(){}

void Light::init(){
  std::list<Light_*>* list_light = data->get_list_light();
  //---------------------------

  //LIGHT Class est build avant LOAD class -> problem
  //Il faut rendre loader totalement independant !

  loaderManager->load_object("../media/engine/Marks/sphere.obj");
  Light_* light = (Light_*)loaderManager->get_created_object();
  light->name = "Light";
  light->is_visible = true;
  transformManager->make_scaling(light, 0.1);
  sceneManager->update_buffer_location(light);
  sceneManager->update_glyph(light);
  list_light->push_back(light);

  //---------------------------
}
