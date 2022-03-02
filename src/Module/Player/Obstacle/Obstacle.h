#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../../common.h"

class Scene;
class Glyphs;
class Saver;
class Interface_node;
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
  void add_prediction();
  void add_prediction(string path_dir);
  void add_groundTruth();

  //Subfunctions
  void build_obstacleGlyph_gt(Cloud* cloud);
  void build_obstacleGlyph_gt(Subset* subset);
  void build_obstacleGlyph_pr(Cloud* cloud);
  void build_obstacleGlyph_pr(Subset* subset);

private:
  Scene* sceneManager;
  OOBB* oobbManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Interface_node* node_interface;
  Capture* captureManager;
  Prediction* predManager;
  Pather* pathManager;

  vec4 gt_color;
  vec4 pr_color;
};

#endif
