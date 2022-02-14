#ifndef WIN_CLOUD_H
#define WIN_CLOUD_H

#include "../../common.h"

class Scene;
class Attribut;


class WIN_cloud
{
public:
  //Constructor / Destructor
  WIN_cloud();
  ~WIN_cloud();

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
};

#endif
