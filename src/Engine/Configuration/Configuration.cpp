#include "Configuration.h"

#include "config_opengl.h"
#include "config_module.h"


//Constructor / Destructor
Configuration::Configuration(){
  //---------------------------

  this->conf_glManager = new config_opengl();
  this->conf_modManager = new config_module();

  //---------------------------
}
Configuration::~Configuration(){}

//Main functions
void Configuration::make_configuration(){
  //---------------------------

  conf_glManager->make_configuration();
  conf_modManager->make_configuration();

  //---------------------------
}
