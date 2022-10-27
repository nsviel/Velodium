#ifndef MOD_CLOUD_H
#define MOD_CLOUD_H

#include "../../common.h"

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
  void window_cloudInfo();
  void window_asciiData();

  //Sub functions
  void cloud_stats_location(Cloud* cloud);
  void cloud_stats_intensity(Cloud* cloud);
  void cloud_stats_distance(Cloud* cloud);
  void cloud_stats_cosIt(Cloud* cloud);

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Color* colorManager;
};

#endif
