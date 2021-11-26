#ifndef LOADER_H
#define LOADER_H

#include "../common.h"
#include "../Engine/Data/struct_dataFile.h"

class dataExtraction;


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

  //Specific formats
  dataFile* OBJLoader(string pathFile);
  dataFile* XYZLoader(string pathFile);

  inline Cloud* get_createdcloud(){return cloud;}

private:
  dataExtraction* extractManager;
  Cloud* cloud;
};

#endif
