#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Glyphs;
class Saver;
class Obstacle_IO;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle();
  ~Obstacle();

public:
  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_pr(Cloud* cloud);

  void select_dir_path();

  inline Obstacle_IO* get_ioManager(){return ioManager;}

private:
  Glyphs* glyphManager;
  Obstacle_IO* ioManager;
};

#endif
