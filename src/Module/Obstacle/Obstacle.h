#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Scene;
class Glyphs;
class Saver;
class Interfacing;
class OOBB;
class Transforms;
class Module_node;
class Capture;


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

private:
  Scene* sceneManager;
  OOBB* oobbManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Interfacing* ioManager;
  Capture* captureManager;

  bool save_frame;
  bool save_image;
};

#endif
