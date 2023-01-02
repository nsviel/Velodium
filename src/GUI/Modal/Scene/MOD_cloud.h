#ifndef MOD_CLOUD_H
#define MOD_CLOUD_H

#include "../../../common.h"

class Node_operation;
class Scene;
class Color;
class Attribut;


class MOD_cloud
{
public:
  //Constructor / Destructor
  MOD_cloud(Node_operation* node_ope);
  ~MOD_cloud();

public:
  //Main function
  void mod_cloud_info();
  void mod_cloud_data();

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Color* colorManager;
};

#endif
