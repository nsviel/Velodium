#ifndef INTERFACE_SAVING_H
#define INTERFACE_SAVING_H

#include "../../common.h"

#include <thread>
#include <queue>

class Node_operation;
class Engine_node;
class Scene;
class Renderer;
class Configuration;
class Saver;
class Capture;


class Saving
{
public:
  //Constructor / Destructor
  Saving(Node_operation* node_ope);
  ~Saving();

public:
  void update_configuration();
  void update_dynamic(Cloud* cloud, int ID_subset);
  void controler_nb_subset(Cloud* cloud);
  void clean_directories();
  void check_directories();

  //Output: frame & Image saving
  void save_image();
  void save_image_unique();
  void save_image_multiple();
  void save_image_path();
  void save_frame(Subset* subset);

  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}
  inline string get_path_image(){return path_image;}
  inline string get_path_image_last(){return path_image_last;}
  inline string get_path_frame(){return path_frame;}
  inline int* get_save_frame_max(){return &save_frame_max;}
  inline int* get_save_image_max(){return &save_image_max;}

private:
  Scene* sceneManager;
  Renderer* renderManager;
  Configuration* configManager;
  Saver* saverManager;
  Capture* captureManager;

  queue<string> save_frame_vec;
  queue<string> save_image_vec;

  string path_dir;
  string path_frame;
  string path_image;
  string path_image_last;
  float time_save_image;
  float time_save_frame;
  int save_frame_max;
  int save_image_ID;
  int save_image_max;

  int nb_subset_max;
  bool with_justOneFrame;
  bool with_save_frame;
  bool with_save_image;

  std::thread thread_frame;
};

#endif
