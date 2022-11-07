#ifndef ONTHEFLY_H
#define ONTHEFLY_H

#include "../../common.h"

class Node_operation;
class Scene;
class Loader;
class Configuration;


class Onthefly
{
public:
  //Constructor / Destructor
  Onthefly(Node_operation* node_ope);
  ~Onthefly();

public:
  void update_configuration();
  void compute_onthefly(Cloud* cloud, int ID);

private:
  Scene* sceneManager;
  Loader* loadManager;
  Configuration* configManager;

  int range;
};

#endif