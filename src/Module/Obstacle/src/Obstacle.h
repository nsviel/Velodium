#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../../common.h"

class Module_obstacle;
class Scene;
class Object;
class Warning;
class OOBB;
class Pose;
class Prediction;
class Pather;


//BIEN CHECKER si les OOBB des preditions snot bien deleted

class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle(Module_obstacle* node);
  ~Obstacle();

public:
  //Main functions
  void runtime();

  //Manual obstacle adding
  void add_detectioned();
  void add_detectioned(string path_dir);
  void add_obstacle_grTr();

  //Subfunctions
  void build_cloud_obstacle(Cloud* cloud);
  void build_obstacleGlyph_gt(Subset* subset);
  void build_obstacleGlyph_pr(Subset* subset);

  inline bool* get_with_warning(){return &with_warning;}
  inline bool* get_with_prediction(){return &with_prediction;}

private:
  Pose* poseManager;
  Scene* sceneManager;
  OOBB* oobbManager;
  Warning* warningManager;
  Object* objectManager;
  Prediction* predManager;
  Pather* pathManager;

  bool with_warning;
  bool with_prediction;
};

#endif
