#ifndef PATHER_H
#define PATHER_H

#include "../../common.h"

class Load_node;
class Scene;
class Loader;
class Saver;
class Configuration;


class Pather
{
public:
  //Constructor / Destructor
  Pather(Load_node* node_load);
  ~Pather();

public:
  void update_configuration();

  //Loading function
  void loading();
  void loading_cloud();
  void loading_frames();
  void loading_directory_frame(string path);
  void loading_sampling();
  void loading_treatment();
  void loading_sampledCloud(string path);
  void loading_fastScene(int mode);

  //Saving functions
  void saving();
  void saving_cloud_frame(Cloud* cloud);
  void saving_subset(Subset* subset);
  void saving_subset_range(int frame_b, int frame_e);
  void saving_cloud(Cloud* cloud);
  void saving_cloud_all();
  void saving_saved_frames();

  //Specific functions
  void convertIscale();
  void selectDirectory(string& path);
  string get_filePath();
  string get_filePath(string path);
  vector<string> get_directoryAllFilePath(string path);

  //Zenity stuff
  vector<string> zenity_loading(string title);
  string zenity_saving(string filename);
  string zenity_directory();
  void zenity_select_directory(string& path_dir);

  inline void set_spaceSampling(float value){this->spaceSampling = value;}
  inline void set_nbLineSampling(int value){this->nbLineSampling = value;}
  inline Loader* get_loaderManager(){return loaderManager;}
  inline string get_open_mode(){return open_mode;}
  inline string get_save_mode(){return save_mode;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Saver* saverManager;
  Configuration* configManager;

  float spaceSampling;
  int nbLineSampling;
  string open_mode;
  string save_mode;
  string path_saved_frame;
  string path_current_dir;
  uint modelID, comID;
};

#endif
