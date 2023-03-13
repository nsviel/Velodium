#ifndef LOADER_H
#define LOADER_H

#include "../../Scene/Data/Data.h"
#include "../../Scene/Base/struct_data_file.h"
#include "../../common.h"

class Extractor;
class Scene;

class file_PTS;
class file_PLY;
class file_PTX;
class file_CSV;
class file_OBJ;
class file_XYZ;
class file_LAS;
class file_PCAP;
class file_CBOR;


class Loader
{
public:
  //Constructor / Destructor
  Loader();
  ~Loader();

public:
  //Main functions
  Collection* load_collection(string pathFile);
  Object_* load_object(string path);
  bool load_cloud_byFrame(vector<string> path_vec);
  bool load_cloud_onthefly(vector<string> path_vec);
  bool load_cloud_silent(string pathFile);
  bool load_cloud_part(string path, int lmin, int lmax);
  bool load_cloud_creation(Collection* cloud_in);
  bool load_cloud_empty();
  bool load_cloud_oneFrame(Collection* collection, string path);
  vector<vec3> load_vertices(string path);

  inline Collection* get_created_object(){return collection;}
  inline file_PTS* get_ptsManager(){return ptsManager;}
  inline file_PTX* get_ptxManager(){return ptxManager;}
  inline file_PCAP* get_pcapManager(){return pcapManager;}

private:
  Data_file* load_retrieve_data(string path);
  vector<Data_file*> load_retrieve_cloud_data(string path);
  Object_* load_insertIntoDatabase(Data_file* data_file);
  Collection* load_insertIntoDatabase(vector<Data_file*> data_vec);
  void load_insertIntoCloud(Data_file* data, Collection* collection);

private:
  Extractor* extractManager;
  Scene* sceneManager;
  Collection* collection;
  Object_ object;
  Data* data;

  file_PTS* ptsManager;
  file_PLY* plyManager;
  file_PTX* ptxManager;
  file_CSV* csvManager;
  file_OBJ* objManager;
  file_XYZ* xyzManager;
  file_LAS* lasManager;
  file_PCAP* pcapManager;
  file_CBOR* cborManager;
};

#endif
