#include "Module.h"


//Constructor / Destructor
Module::Module(){
  //---------------------------


  //---------------------------
}
Module::~Module(){}

void Module::module_configuration(){
  //-------------------------------

  this->configManager = new config_module();
  configManager->make_configuration();

  //-------------------------------
}
void Module::module_update(){
  //-------------------------------

  //gui_playerManager->update();

  //-------------------------------
}
void Module::module_runtime(){
  //-------------------------------

  //gui_playerManager->player_mouse();
  //gui_obstacleManager->online_run();

  //-------------------------------
}
