#ifndef INTERFACE_GPS_H
#define INTERFACE_GPS_H

#include "../../../common.h"

#include <thread>

class Scene;
class Interface_node;


class GPS
{
public:
  //Constructor / Destructor
  GPS(Interface_node* node);
  ~GPS();

public:
  //Input: GPS position
  void runtime_gps();

  //Subfunctions
  void parse_json_gps(string path);

  inline string get_path_gps(){return path_gps;}
  inline bool* get_with_gps(){return &with_gps;}

private:
  Scene* sceneManager;

  string path_gps;

  bool is_gps;
  bool with_gps;
};

#endif
