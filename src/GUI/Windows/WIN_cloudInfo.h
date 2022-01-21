#ifndef WIN_CLOUDINFO_H
#define WIN_CLOUDINFO_H

#include "../../common.h"

class Scene;
class Attribut;


class WIN_cloudInfo
{
public:
  //Constructor / Destructor
  WIN_cloudInfo();
  ~WIN_cloudInfo();

public:
  //Main function
  void window_cloudInfo();

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
