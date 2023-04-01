#ifndef RECORDER_H
#define RECORDER_H

#include "../../common.h"

#include <thread>
#include <queue>

class Node_interface;
class Node_engine;
class Node_operation;

class Renderer;
class GPU_screenshot;
class Configuration;
class Saver;
class Scene;


class Recorder
{
public:
  //Constructor / Destructor
  Recorder(Node_interface* node);
  ~Recorder();

public:
  //Main functions
  void update_configuration();
  void compute_online(Collection* collection, int ID_object);
  void clean_directories();
  void check_directories();

  //Image saving
  void save_image();
  void save_image_unique();
  void save_image_multiple();
  void save_image_path();

  //Frame saving
  void save_frame(Collection* collection, int ID_object);
  void save_frame_subset(Cloud* cloud);
  void save_frame_set(Collection* collection, int ID_object);

  //Path selection
  void select_path_image();
  void select_path_frame();
  void select_image_unlimited(bool value);
  void select_frame_unlimited(bool value);

  inline string get_path_image(){return path_image;}
  inline string get_path_frame(){return path_frame;}
  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline bool* get_with_save_frame_raw(){return &with_save_frame_raw;}
  inline int* get_save_frame_max(){return &save_frame_max;}
  inline int* get_save_image_max(){return &save_image_max;}
  inline int* get_save_frame_accu(){return &save_frame_accu;}

private:
  Node_interface* node_interface;
  Renderer* renderManager;
  GPU_screenshot* screenshotManager;
  Configuration* configManager;
  Saver* saverManager;
  Scene* sceneManager;

  string path_dir;
  string path_frame;
  string path_image;
  float time_save_frame;
  int save_frame_max;
  int save_frame_accu;
  int save_image_ID;
  int save_image_max;
  bool with_save_frame;
  bool with_save_image;
  bool with_save_frame_raw;

  queue<string> save_frame_vec;
  queue<string> save_image_vec;
  std::thread thread_frame;
};

#endif
