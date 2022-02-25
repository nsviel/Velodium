#ifndef WIN_LOADING_H
#define WIN_LOADING_H

#include "../../common.h"

class Engine_node;
class Scene;
class Pather;
class Loader;


class WIN_loading
{
public:
  //Constructor / Destructor
  WIN_loading(Engine_node* node_engine);
  ~WIN_loading();

public:
  //Main function
  void window_loading();
  void window_saving();

  //Sub functions
  void loading_action();
  void loading_dataFormat();
  void loading_fileSelection();
  void loading_retrieve_info(string file_path);
  void loading_file_ptx();
  void loading_file_pcap();

  void saving_action();
  void saving_configuration();

private:
  Scene* sceneManager;
  Pather* pathManager;
  Loader* loaderManager;

  string file_path;
  string file_format;
  bool file_selected;
  int item_width;
};

#endif
