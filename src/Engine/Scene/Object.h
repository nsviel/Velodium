#ifndef OBJECT_H
#define OBJECT_H

#include "../../common.h"

class Node_engine;
class Configuration;
class Glyphs;

class Grid;
class Axis;
class AABB;
class Normal;
class OOBB;
class Mark;
class Slam_keypoint;
class Car;
class Trajectory;
class Localmap;


class Object
{
public:
  //Constructor / Destructor
  Object(Node_engine* node);
  ~Object();

public:
  //Runtime functions
  void runtime_glyph_scene();
  void runtime_glyph_subset_all(Cloud* cloud);
  void runtime_glyph_subset_selected(Subset* subset);
  void runtime_glyph_pred(Subset* subset);

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
  void set_object_visibility(string name, bool val);
  void set_slam_object(bool value);
  void create_glyph_scene();
  void create_glyph_subset(Subset* subset);
  Glyph* create_glyph_ostacle();

  inline Grid* get_object_grid(){return gridObject;}
  inline Axis* get_object_axis(){return axisObject;}
  inline AABB* get_object_aabb(){return aabbObject;}
  inline Normal* get_object_normal(){return normObject;}
  inline Mark* get_object_mark(){return markObject;}
  inline Trajectory* get_object_trajectory(){return trajObject;}
  inline Car* get_object_car(){return carObject;}
  inline Slam_keypoint* get_object_keypoint(){return keyObject;}
  inline Localmap* get_object_localmap(){return mapObject;}

private:
  Node_engine* node_engine;
  Glyphs* glyphManager;
  Configuration* configManager;
  Grid* gridObject;
  Axis* axisObject;
  AABB* aabbObject;
  Normal* normObject;
  Slam_keypoint* keyObject;
  OOBB* oobbObject;
  Trajectory* trajObject;
  Localmap* mapObject;
  Mark* markObject;
  Car* carObject;
};

#endif
