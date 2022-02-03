#ifndef MODULE_H
#define MODULE_H

#include "../Engine/Configuration/config_module.h"
#include "../common.h"


class Module
{
public:
  //Constructor / Destructor
  Module();
  ~Module();

public:
  void module_configuration();
  void module_update();
  void module_runtime();

private:
  config_module* configManager;
};

#endif
