#ifndef WIN_MODIFYFILEINFO_H
#define WIN_MODIFYFILEINFO_H

#include "../../common.h"

class Scene;
class Attribut;


class WIN_modifyFileInfo
{
public:
  //Constructor / Destructor
  WIN_modifyFileInfo();
  ~WIN_modifyFileInfo();

public:
  //Main function
  void window_modifyFileInfo();

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
