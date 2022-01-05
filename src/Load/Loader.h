#ifndef LOADER_H
#define LOADER_H

#include "struct_dataFile.h"

#include "../common.h"

class dataExtraction;
class Scene;

class file_PTS;
class file_PLY;
class file_PTX;
class file_CSV;
class file_OBJ;
class file_XYZ;
class file_PCAP;


class Loader
{
public:
  //Constructor / Destructor
  Loader();
  ~Loader();

public:
  //Loading functions
  bool load_cloud(string pathFile);
  bool load_cloud_byFrame(vector<string> path_vec);
  bool load_cloud_silent(string pathFile);
  bool load_cloud_part(string path, int lmin, int lmax);
  bool load_cloud_creation(Cloud* cloud_in);
  bool load_cloud_empty();
  bool load_cloud_oneFrame();

  vector<dataFile*> load_retrieve_data(string filePath);
  void load_insertIntoDatabase(vector<dataFile*> data_vec);

  //Saving functions
  bool save_cloud(Cloud* cloud_in, string pathFile);
  bool save_subset(Subset* subset, string format, string dirPath);
  bool save_subset(Subset* subset, string format, string dirPath, string fileName);

  inline Cloud* get_createdcloud(){return cloud;}
  inline file_PTS* get_ptsManager(){return ptsManager;}
  inline file_PTX* get_ptxManager(){return ptxManager;}
  inline file_PCAP* get_pcapManager(){return pcapManager;}

private:
  dataExtraction* extractManager;
  Scene* sceneManager;
  Cloud* cloud;

  file_PTS* ptsManager;
  file_PLY* plyManager;
  file_PTX* ptxManager;
  file_CSV* csvManager;
  file_OBJ* objManager;
  file_XYZ* xyzManager;
  file_PCAP *pcapManager;
};

#endif
