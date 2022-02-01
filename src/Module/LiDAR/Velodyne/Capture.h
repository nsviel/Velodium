#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../../common.h"
#include "UDP/struct_UDPpacket.h"

#include <thread>

class Scene;
class Loader;
class dataExtraction;


class Capture
{
public:
  //Constructor / Destructor
  Capture();
  ~Capture();

public:
  //Recording functions
  void check_forNewSubset();

  void new_capture();
  void create_subset(udpPacket* frame_in, bool is_recording);
  void recording_selectDirSave();

  inline string* get_path_frameSave(){return &path_frameSave;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  dataExtraction* extractManager;

  Cloud* cloud_capture;
  Subset* subset_capture;

  int ID_capture;
  int ID_subset;
  string path_frameSave;
  string path_dirSave;
  bool oneFrame;
  bool atLeastOne;
};

#endif
