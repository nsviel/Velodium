#ifndef INTERFACE_GPS_H
#define INTERFACE_GPS_H

#include "../../../common.h"

#include <thread>

class Engine_node;


class GPS
{
public:
  //Constructor / Destructor
  GPS(Engine_node* node_engine);
  ~GPS();

public:
  //Input: GPS position
  void start_watcher_gps();
  void stop_watcher_gps();

  //Subfunctions
  bool runtime_gps();
  void parse_json_gps(string path);

  inline string get_path_gps(){return path_gps;}
  inline bool get_is_watching(){return is_whatching;}

private:
  string path_dir;
  string path_gps;
  string path_gps_file;

  bool thread_gps_ON;
  bool is_whatching;
  bool flag_newGPS;

  std::thread thread_gps;
};

#endif
