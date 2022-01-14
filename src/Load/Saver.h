#ifndef SAVER_H
#define SAVER_H

#include "Format/struct_dataFile.h"

#include "../common.h"

class Scene;

class file_PTS;
class file_PLY;
class file_PTX;
class file_CSV;
class file_OBJ;
class file_XYZ;
class file_PCAP;


class Saver
{
public:
  //Constructor / Destructor
  Saver();
  ~Saver();

public:
  //Main functions
  bool save_cloud(Cloud* cloud_in, string pathFile);
  bool save_subset(Subset* subset, string format, string dirPath);
  bool save_subset(Subset* subset, string format, string dirPath, string fileName);

  inline file_PTS* get_ptsManager(){return ptsManager;}
  inline file_PTX* get_ptxManager(){return ptxManager;}
  inline file_PCAP* get_pcapManager(){return pcapManager;}

private:
  Scene* sceneManager;

  file_PTS* ptsManager;
  file_PLY* plyManager;
  file_PTX* ptxManager;
  file_CSV* csvManager;
  file_OBJ* objManager;
  file_XYZ* xyzManager;
  file_PCAP *pcapManager;
};

#endif
