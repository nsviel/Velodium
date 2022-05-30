#ifndef OBJECT_H
#define OBJECT_H

#include "../../common.h"

class Engine_node;
class Configuration;
class Glyphs;

class Grid;
class Axis;
class AABB;
class Normal;
class OOBB;
class Mark;
class Keypoint;
class Car;
class Trajectory;


class Object
{
public:
  //Constructor / Destructor
  Object(Engine_node* node);
  ~Object();

public:
  //Runtime functions
  void runtime_glyph_scene();
  void runtime_glyph_pred(Cloud* cloud, int subset_ID);
  void runtime_subset_object(Subset* subset);
  void runtime_object_selected(Subset* subset);

  //Update functions
  void update_configuration();
  void update_dynamic(Cloud* cloud);
  void update_glyph_subset(Subset* subset);
  void update_glyph_cloud(Cloud* cloud);
  void update_object(Glyph* glyph);
  void update_object(Glyph* glyph, vec4 color);

  //Reset functions
  void reset_scene_object();
  void reset_color_object();
  void reset_object(Glyph* glyph);

  //Misc functions
  void init_object();
  void set_object_visibility(Cloud* cloud, string name, bool val);
  void create_glyph_subset(Subset* subset);
  Glyph* create_glyph_ostacle();

  inline Grid* get_object_grid(){return gridObject;}
  inline Axis* get_object_axis(){return axisObject;}
  inline AABB* get_object_aabb(){return aabbObject;}
  inline Normal* get_object_normal(){return normObject;}
  inline Mark* get_object_mark(){return markObject;}
  inline Trajectory* get_object_trajectory(){return trajObject;}
  inline Car* get_object_car(){return carObject;}
  inline Keypoint* get_object_keypoint(){return keyObject;}

private:
  Engine_node* node_engine;
  Glyphs* glyphManager;
  Configuration* configManager;
  Grid* gridObject;
  Axis* axisObject;
  AABB* aabbObject;
  Normal* normObject;
  Keypoint* keyObject;
  OOBB* oobbObject;
  Trajectory* trajObject;
  Mark* markObject;
  Car* carObject;
};

#endif
