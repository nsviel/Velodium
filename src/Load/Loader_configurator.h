#ifndef LOADER_CONFIGURATOR_H
#define LOADER_CONFIGURATOR_H

#include "../common.h"

class filePTS;
class filePLY;
class filePTX;


class Loader_configurator
{
public:
  //Constructor / Destructor
  Loader_configurator();
  ~Loader_configurator();

public:
  void load_option(int format, int option, bool value);

private:
  filePTS* ptsManager;
  filePLY* plyManager;
  filePTX* ptxManager;
};

#endif
