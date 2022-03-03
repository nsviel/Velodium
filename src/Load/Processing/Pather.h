#ifndef PATHER_H
#define PATHER_H

#include "../../common.h"

class Load_node;
class Scene;
class Loader;
class Saver;


class Pather
{
public:
  //Constructor / Destructor
  Pather(Load_node* node_load);
  ~Pather();

public:
  //Loading function
  void loading();
  void loading_frames();
  void loading_directoryFrames(string path);
  void loading_sampling();
  void loading_treatment();
  void loading_sampledCloud(string path);
  void loading_fastScene(int mode);

  //Saving functions
  void saving_subset(Subset* subset);
  void saving_subset_range(int frame_b, int frame_e);
  void saving_cloud(Cloud* cloud);
  void saving_cloud_all();

  //Specific functions
  void convertIscale();
  void selectDirectory(string& path);
  string get_filePath();
  string get_filePath(string path);
  vector<string> get_directoryAllFilePath(string path);

  //Zenity stuff
  vector<string> zenity_loading(string title);
  string zenity_saving();
  string zenity_directory();
  void zenity_select_directory(string& path_dir);

  inline void set_spaceSampling(float value){this->spaceSampling = value;}
  inline void set_nbLineSampling(int value){this->nbLineSampling = value;}
  inline Loader* get_loaderManager(){return loaderManager;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Saver* saverManager;

  float spaceSampling;
  int nbLineSampling;
  string path_current_dir;
  uint modelID, comID;
};

#endif