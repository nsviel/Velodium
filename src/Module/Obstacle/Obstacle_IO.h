#ifndef OBSTACLE_IO_H
#define OBSTACLE_IO_H

#include "../../common.h"

#include <thread>

class Scene;
class Saver;
class Operation;
class Module_node;
class Renderer;


class Obstacle_IO
{
public:
  //Constructor / Destructor
  Obstacle_IO(Module_node* node_module);
  ~Obstacle_IO();

public:
  void start_dirWatcher();
  void stop_dirWatcher();

  //Save functions
  void save_image(Subset* subset);
  void save_image_path();

  bool check_obstacleData();
  void clean_directories();
  void save_nFrame(Cloud* cloud);
  void parse_obstacle_json(Cloud* cloud, string paths, string data);
  void select_dir_path();

  inline string get_dir_path(){return path_dir;}
  inline string* get_save_image_path(){return &screenshot_path;}
  inline bool* get_is_thread_pred(){return &thread_predi_ON;}

private:
  Scene* sceneManager;
  Saver* saverManager;
  Operation* opeManager;
  Renderer* renderManager;

  vector<string> save_path_vec;
  string path_dir;
  string path_predi;
  string path_grThr;
  string path_frame;
  string path_file_grThr;
  string path_file_predi;
  string screenshot_path;
  int savedFrame_ID;
  int savedFrame_max;

  bool thread_predi_ON;
  bool thread_grThr_ON;
  bool flag_newPred;
  bool flag_newGrTh;

  std::thread thread_predi;
  std::thread thread_grThr;
};

#endif
