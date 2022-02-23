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
class Prediction;
class Pather;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle(Module_node* node_module);
  ~Obstacle();

public:
  //Main functions
  void runtime_obstacle();
  void compute_obstacle();
  void compute_groundTruth();

  //Subfunctions
  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_pr(Cloud* cloud);

private:
  Scene* sceneManager;
  OOBB* oobbManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Interfacing* ioManager;
  Capture* captureManager;
  Prediction* predManager;
  Pather* pathManager;
};

#endif
