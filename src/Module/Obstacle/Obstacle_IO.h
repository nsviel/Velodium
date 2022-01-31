#ifndef OBSTACLE_IO_H
#define OBSTACLE_IO_H

#include "../../common.h"

class Saver;


class Obstacle_IO
{
public:
  //Constructor / Destructor
  Obstacle_IO();
  ~Obstacle_IO();

public:
  void Load_obstacleData();
  void clean_directories();
  void save_nFrame(Cloud* cloud);
  void parse_json_obstacle(Cloud* cloud, vector<string> paths, string data);
  void select_dir_path();

  inline string get_dir_path(){return dir_path;}

private:
  Saver* saverManager;

  vector<string> save_path_vec;
  string dir_path;
  string dir_predi;
  string dir_grThr;
  string dir_frame;
  int savedFrame_ID;
  int savedFrame_max;
};

#endif
