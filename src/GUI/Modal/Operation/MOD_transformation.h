#ifndef MOD_TRANSFORMATION_H
#define MOD_TRANSFORMATION_H

#include "../../../common.h"

class Node_engine;
class Scene;
class Glyphs;
class Transforms;


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
  void cloud_translation();
  void cloud_rotation();
  void cloud_elevation();
  void cloud_alignment();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Transforms* transformManager;

  int item_width;
};

#endif
