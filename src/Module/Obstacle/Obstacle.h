#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Glyphs;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle();
  ~Obstacle();

public:
  void run();

  void parse_json(Cloud* cloud, vector<string> paths_pr, string data);
  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_pr(Cloud* cloud);

  void select_dir_path();

  inline string get_dir_path(){return dir_path;}

private:
  Glyphs* glyphManager;

  string dir_path;
  string dir_predi;
  string dir_grThr;
  string dir_frame;
};

#endif
