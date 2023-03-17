#ifndef SAVER_H
#define SAVER_H

#include "../../Scene/Base/struct_data_file.h"
#include "../../common.h"

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
  bool save_collection(Collection* cloud_in, string pathFile);
  bool save_subset(Cloud* cloud, string format, string dirPath);
  bool save_subset(Cloud* cloud, string format, string dirPath, string fileName);
  bool save_subset_silent(Cloud* cloud, string format, string dirPath);
  bool save_set_silent(Collection* collection, int ID, string path, int nb);

  inline file_PTS* get_ptsManager(){return ptsManager;}
  inline file_PTX* get_ptxManager(){return ptxManager;}
  inline file_PCAP* get_pcapManager(){return pcapManager;}

private:
  file_PTS* ptsManager;
  file_PLY* plyManager;
  file_PTX* ptxManager;
  file_CSV* csvManager;
  file_OBJ* objManager;
  file_XYZ* xyzManager;
  file_PCAP *pcapManager;
};

#endif
