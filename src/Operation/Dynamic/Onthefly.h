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
  void compute_onthefly(Collection* collection, int ID);
  void reset();

  inline int get_range(){return range;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Configuration* configManager;

  int range;
};

#endif
