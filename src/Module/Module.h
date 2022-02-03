#ifndef MODULE_H
#define MODULE_H

#include "../Engine/Configuration/config_module.h"
#include "../common.h"


class Module
{
public:
  //Constructor / Destructor
  Module(){
    //-------------------------------

    this->module_configuration();

    //-------------------------------
  }
  ~Module();

public:
  void module_configuration(){
    //-------------------------------

    this->configManager = new config_module();
    configManager->make_configuration();

    //-------------------------------
  }
  void module_update(){
    //-------------------------------

    gui_playerManager->update();

    //-------------------------------
  }
  void module_runtime(){
    //-------------------------------

    gui_playerManager->player_mouse();
    gui_obstacleManager->online_run();

    //-------------------------------
  }

private:
  config_module* configManager;
};

#endif
