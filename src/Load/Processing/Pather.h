#ifndef PATHER_H
#define PATHER_H

#include "../../Scene/Data/Data.h"
#include "../../common.h"

class Node_load;
class Scene;
class Loader;
class Saver;
class Configuration;


class Pather
{
public:
  //Constructor / Destructor
  Pather();
  Pather(Node_load* node);
  ~Pather();

public:
  void update_configuration();

  //Onthefly stuff
  bool check_folder_format(string path, string format);
  Collection* loading_onthefly(string path);
  Collection* loading_directory_frame(string path);

  //Loading function
  void loading_cloud();
  void loading_frame();
  void loading_onthefly();

  //Specific loader function
  void loading_sampling();
  void loading_treatment();
  void loading_sampledCloud(string path);
  void loading_fastScene(int mode);

  //Saving functions
  void saving_cloud_frame(Collection* collection);
  void saving_subset(Cloud* cloud);
  void saving_subset_range(int frame_b, int frame_e);
  void saving_cloud(Collection* collection);
  void saving_cloud_same(Collection* collection);
  void saving_cloud_all();
  void saving_saved_frames();

  inline void set_spaceSampling(float value){this->spaceSampling = value;}
  inline void set_nbLineSampling(int value){this->nbLineSampling = value;}
  inline Loader* get_loaderManager(){return loaderManager;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Saver* saverManager;
  Configuration* configManager;
  Data* data;

  float spaceSampling;
  int nbLineSampling;
  string path_saved_frame;
  string path_current_dir;
  uint modelID, comID;
};

#endif
