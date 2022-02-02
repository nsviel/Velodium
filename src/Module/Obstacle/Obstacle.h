#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Glyphs;
class Saver;
class Obstacle_IO;
class OOBB;
class Transforms;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle();
  ~Obstacle();

public:
  void online_run(Cloud* cloud);

  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_pr(Cloud* cloud);

  inline Obstacle_IO* get_ioManager(){return ioManager;}

private:
  OOBB* oobbManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Obstacle_IO* ioManager;
};

#endif
