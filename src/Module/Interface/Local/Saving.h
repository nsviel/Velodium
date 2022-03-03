#ifndef INTERFACE_SAVING_H
#define INTERFACE_SAVING_H

#include "../../../common.h"

#include <thread>
#include <queue>

class Engine_node;
class Saver;
class Renderer;


class Saving
{
public:
  //Constructor / Destructor
  Saving(Engine_node* node_engine);
  ~Saving();

public:
  //Output: frame & Image saving
  void save_image();
  void save_image_path();
  void save_frame(Subset* subset);

  inline string get_path_image(){return path_image;}
  inline string get_path_frame(){return path_frame;}
  inline int* get_save_frame_max(){return &save_frame_max;}
  inline int* get_save_image_max(){return &save_image_max;}

private:
  Saver* saverManager;
  Renderer* renderManager;

  queue<string> save_frame_vec;
  queue<string> save_image_vec;

  string path_dir;
  string path_frame;
  string path_image;

  int save_frame_max;
  int save_image_ID;
  int save_image_max;

  std::thread thread_frame;
};

#endif
