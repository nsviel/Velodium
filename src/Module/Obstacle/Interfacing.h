#ifndef INTERFACING_H
#define INTERFACING_H

#include "../../common.h"

#include <thread>
#include <queue>

class Scene;
class Saver;
class Pather;
class Module_node;
class Renderer;


class Interfacing
{
public:
  //Constructor / Destructor
  Interfacing(Module_node* node_module);
  ~Interfacing();

public:
  //Input: Prediction stuff
  void start_dirWatcher();
  void stop_dirWatcher();
  bool check_prediction(Cloud* cloud);
  void parse_obstacle_json(Cloud* cloud, string paths, string data);

  //Output: frame & Image saving
  void save_image();
  void save_image_path();
  void save_frame(Subset* subset);

  //Subfunctions
  void clean_directories();
  void select_dir_path();

  inline string get_dir_path(){return path_dir;}
  inline string* get_save_image_path(){return &path_image;}
  inline bool* get_is_thread_pred(){return &thread_predi_ON;}
  inline bool get_is_watching(){return is_whatching;}
  inline int* get_save_frame_max(){return &save_frame_max;}
  inline int* get_save_image_max(){return &save_image_max;}

private:
  Scene* sceneManager;
  Saver* saverManager;
  Pather* pathManager;
  Renderer* renderManager;

  queue<string> save_frame_vec;
  queue<string> save_image_vec;
  string path_dir;
  string path_predi;
  string path_predi_file;
  string path_grThr;
  string path_grThr_file;
  string path_frame;
  string path_image;

  int save_frame_max;
  int save_image_ID;
  int save_image_max;

  bool thread_predi_ON;
  bool thread_grThr_ON;
  bool flag_newPred;
  bool flag_newGrTh;
  bool is_whatching;

  std::thread thread_predi;
  std::thread thread_grThr;
};

#endif
