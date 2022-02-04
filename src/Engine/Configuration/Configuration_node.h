#ifndef NODE_CONFIGURATION_H
#define NODE_CONFIGURATION_H

#include "config_opengl.h"
#include "config_module.h"

#include "../../common.h"


class Configuration_node
{
public:
  //Constructor / Destructor
  Configuration_node(){
    this->make_configuration();
  }
  ~Configuration_node();

public:
  //Main functions
  void make_configuration(){
    //---------------------------

    this->conf_glManager = new config_opengl();
    this->conf_modManager = new config_module();

    conf_glManager->make_configuration();
    conf_modManager->make_configuration();

    //---------------------------
  }

  inline config_opengl* get_conf_glManager(){return conf_glManager;}
  inline config_module* get_conf_modManager(){return conf_modManager;}

private:
  config_opengl* conf_glManager;
  config_module* conf_modManager;
};

#endif