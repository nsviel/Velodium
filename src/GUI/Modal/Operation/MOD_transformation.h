#ifndef MOD_TRANSFORMATION_H
#define MOD_TRANSFORMATION_H

#include "../../../common.h"

class Node_engine;
class Scene;
class Glyphs;
class Transformation;
class Pose;
class GPU_data;


class MOD_transformation
{
public:
  //Constructor / Destructor
  MOD_transformation(Node_engine* node_engine);
  ~MOD_transformation();

public:
  void design_transformation();

  //Specifc function
  void actual_transformation_mat();
  void manual_transformation_mat();
  void object_translation();
  void object_rotation();
  void object_elevation();
  
  //Elevation function
  void elevation_ground();
  void elevation_height();
  void elevation_redressment();

private:
  Transformation* transformManager;
  Pose* poseManager;
  Scene* sceneManager;
  Glyphs* glyphManager;
  GPU_data* gpuManager;

  int item_width;
  float ground;
};

#endif
