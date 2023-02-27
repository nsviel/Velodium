#include "Light.h"

#include "../Node_engine.h"
#include "../Scene/Scene.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Pose.h"


//Constructor / Destructor
Light::Light(Node_engine* node){
  //---------------------------

  Node_load* node_load = node->get_node_load();

  this->loaderManager = node_load->get_loaderManager();
  this->sceneManager = node->get_sceneManager();
  this->transformManager = new Transformation();
  this->poseManager = new Pose();

  //---------------------------
}
Light::~Light(){}

void Light::init(){
  //---------------------------

  loaderManager->load_cloud("../media/engine/Marks/sphere.obj");
  Cloud* light = loaderManager->get_createdcloud();
  light->name = "Light";
  transformManager->make_scaling(light, 0.1);
  sceneManager->update_cloud_location(light);
  sceneManager->update_cloud_glyph(light);

  //---------------------------
}
