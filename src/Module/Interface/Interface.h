#ifndef INTERFACE_H
#define INTERFACE_H

#include "../../common.h"

class Interface_node;
class Capture;
class Prediction;
class Saving;
class Capture;
class Scene;
class GPS;


class Interface
{
public:
  //Constructor / Destructor
  Interface(Interface_node* node);
  ~Interface();

public:
  void runtime_loop();
  void update_dynamic(Cloud* cloud, int ID_subset);
  void controler_nb_subset(Cloud* cloud);

  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}

private:
  Capture* captureManager;
  Prediction* predManager;
  Saving* saveManager;
  Scene* sceneManager;
  GPS* gpsManager;

  int nb_subset_max;
  bool with_justOneFrame;
  bool with_save_frame;
  bool with_save_image;
};

#endif
