#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Scene;
class Glyphs;
class Saver;
class Obstacle_IO;
class OOBB;
class Transforms;
class Module_node;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle(Module_node* node_module);
  ~Obstacle();

public:
  void runtime_obstacle();

  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_pr(Cloud* cloud);

  inline Obstacle_IO* get_ioManager(){return ioManager;}

private:
  Scene* sceneManager;
  OOBB* oobbManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Obstacle_IO* ioManager;
};

#endif
