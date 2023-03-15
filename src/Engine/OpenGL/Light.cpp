#include "Light.h"

#include "../GPU/GPU_data.h"
#include "../Shader/Shader.h"
#include "../Shader/Source/Shader_light.h"
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

  this->shaderManager = node->get_shaderManager();
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
  Collection* col_light = data->get_collection_byName("glyph", "light");
  //---------------------------

  Light_* light = (Light_*)loaderManager->load_object("../media/engine/Marks/sphere.obj");
  col_light->obj_add_new(light);

  light->obj_type = "light";
  light->name = "Light";
  light->is_visible = true;
  transformManager->make_scaling(light, 0.1);
  gpuManager->update_buffer_location(light);
  gpuManager->update_buffer_color(light);
  sceneManager->update_MinMax(light);

  //---------------------------
}
void Light::light_being_displaced(){
  Shader_light* shader_light = (Shader_light*)shaderManager->get_shader_src_byName("light");
  Collection* col_light = data->get_collection_byName("glyph", "light");
  //---------------------------

  Object_* light = col_light->get_obj(0);
  shader_light->set_light_position(light->COM);

  //---------------------------
}
