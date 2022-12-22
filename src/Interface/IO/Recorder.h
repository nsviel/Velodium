#ifndef RECORDER_H
#define RECORDER_H

#include "../../common.h"

#include <thread>
#include <queue>

class Node_interface;
class Node_engine;
class Node_operation;

class Renderer;
class Configuration;
class Saver;


class Recorder
{
public:
  //Constructor / Destructor
  Recorder(Node_interface* node);
  ~Recorder();

public:
  void update_configuration();
  void compute_online(Cloud* cloud, int ID_subset);
  void clean_directories();
  void check_directories();

  //Output: frame & Image saving
  void save_image();
  void save_image_unique();
  void save_image_multiple();
  void save_image_path();
  void save_frame(Subset* subset);

  //Path selection
  void select_path_image();
  void select_path_frame();
  void select_image_unlimited(bool value);
  void select_frame_unlimited(bool value);

  inline string get_path_image(){return path_image;}
  inline string get_path_frame(){return path_frame;}
  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline int* get_save_frame_max(){return &save_frame_max;}
  inline int* get_save_image_max(){return &save_image_max;}

private:
  Node_interface* node_interface;
  Renderer* renderManager;
  Configuration* configManager;
  Saver* saverManager;

  string path_dir;
  string path_frame;
  string path_image;
  float time_save_image;
  float time_save_frame;
  int save_frame_max;
  int save_image_ID;
  int save_image_max;
  bool with_save_frame;
  bool with_save_image;

  queue<string> save_frame_vec;
  queue<string> save_image_vec;
  std::thread thread_frame;
};

#endif
