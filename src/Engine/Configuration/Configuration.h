#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../../common.h"

class config_opengl;
class config_module;


class Configuration
{
public:
  //Constructor / Destructor
  Configuration();
  ~Configuration();

public:
  //Main functions
  void make_configuration();

  inline config_opengl* get_conf_glManager(){return conf_glManager;}
  inline config_module* get_conf_modManager(){return conf_modManager;}

private:
  config_opengl* conf_glManager;
  config_module* conf_modManager;
};

#endif
