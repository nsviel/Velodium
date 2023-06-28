#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../common.h"

class Node_interface;
class Node_operation;

class Scene;
class Loader;
class Configuration;
class Scala;
class Velodyne;
class GPU_data;


class Capture
{
public:
  //Constructor / Destructor
  Capture(Node_interface* node_interface);
  ~Capture();

public:
  //Main functions
  void update_configuration();
  void runtime();

  //Start / stop functions
  void start_new_capture(string model);
  void stop_capture();

  //LiDAR specific functions
  void runtime_velodyne();
  void runtime_scala();
  void start_capture_velodyne();
  void start_capture_scala();

  //Subfunctions
  void operation_new_subset(Cloud* cloud);
  void supress_nullpoints(Cloud* cloud);
  void control_nb_subset(Collection* collection);
  void create_empty_cloud();

  inline Scala* get_scalaManager(){return scalaManager;}
  inline Velodyne* get_veloManager(){return veloManager;}
  inline Collection* get_cloud_capture(){return collection_capture;}

  inline string* get_lidar_model(){return &lidar_model;}
  inline bool* get_is_capturing(){return &is_capturing;}
  inline bool* get_is_capture_finished(){return &is_capture_finished;}
  inline int* get_capture_port(){return &capture_port;}
  inline int get_capture_nb_point(){return capture_nb_point;}
  inline int get_capture_nb_point_raw(){return capture_nb_point_raw;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}
  inline int get_ratio_frame(){return ratio_frame;}
  inline int* get_point_size(){return &point_size;}
  inline void set_ratio_frame(int value){this->ratio_frame = value; this->ratio_cpt = 0;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Configuration* configManager;
  Node_operation* node_ope;
  GPU_data* gpuManager;

  Collection* collection_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  string lidar_model;
  bool with_justOneFrame;
  bool with_supress_null;
  bool is_capture_finished;
  bool is_capturing;
  bool is_first_run;
  int capture_port;
  int capture_nb_point;
  int capture_nb_point_raw;
  int ID_capture;
  int ratio_frame;
  int ratio_cpt;
  int nb_subset_max;
  int point_size;
};

#endif
