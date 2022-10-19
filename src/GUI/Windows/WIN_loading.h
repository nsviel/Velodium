#ifndef WIN_LOADING_H
#define WIN_LOADING_H

#include "../../common.h"

class Node_engine;
class Scene;
class Pather;
class Loader;


class WIN_loading
{
public:
  //Constructor / Destructor
  WIN_loading(Node_engine* node_engine);
  ~WIN_loading();

public:
  //Main function
  void window_loading();
  void window_saving();

  //Sub load functions
  void loading_specific();
  void loading_dataFormat();
  void loading_custom_file();
  void loading_retrieve_info(string file_path);
  void loading_file_ptx();
  void loading_file_pcap();
  void loading_action();
  void loading_end(bool* open);

  //Sub save functions
  void saving_configuration();
  void saving_action();
  void saving_dataFormat();
  void saving_end(bool* open);

private:
  Scene* sceneManager;
  Pather* pathManager;
  Loader* loaderManager;

  string file_path;
  string file_format;
  bool file_selected;
  int item_width;
  int save_mode, load_mode;
  int subset_mode, cloud_mode;
  int frame_b, frame_e;
};

#endif
