#ifndef OBJECT_H
#define OBJECT_H

#include "../../../common.h"

class Node_engine;
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
class Localmap;
class Box;
class Tree;
class Matching;


class Object
{
public:
  //Constructor / Destructor
  Object(Node_engine* node);
  ~Object();

public:
  //Creation function
  void create_glyph_scene();
  void create_glyph_subset(Subset* subset);
  Glyph* create_glyph_ostacle();

  //Runtime function
  void runtime_glyph_scene();
  void runtime_glyph_subset_all(Cloud* cloud);
  void runtime_glyph_subset_selected(Subset* subset);
  void runtime_glyph_pred(Subset* subset);

  //Update function
  void update_configuration();
  void update_glyph_subset(Subset* subset);
  void update_glyph_cloud(Cloud* cloud);
  void update_object(Glyph* glyph);
  void update_object(Glyph* glyph, vec4 color);
  void update_object(string obj, vec4 color);

  //Reset function
  void reset_scene_object();
  void reset_color_object();
  void reset_object(Glyph* glyph);

  //Misc function
  void set_object_visibility(string name, bool val);
  void set_slam_object(bool value);
  Glyph* get_glyph_by_name(string name);

  inline Grid* get_object_grid(){return gridObject;}
  inline Axis* get_object_axis(){return axisObject;}
  inline AABB* get_object_aabb(){return aabbObject;}
  inline Normal* get_object_normal(){return normObject;}
  inline Mark* get_object_mark(){return markObject;}
  inline Trajectory* get_object_trajectory(){return trajObject;}
  inline Car* get_object_car(){return carObject;}
  inline Keypoint* get_object_keypoint(){return keyObject;}
  inline Localmap* get_object_localmap(){return mapObject;}
  inline Box* get_object_box(){return boxObject;}
  inline Tree* get_object_tree(){return treeObject;}
  inline Matching* get_object_matching(){return matchObject;}

private:
  Node_engine* node_engine;
  Glyphs* glyphManager;
  Configuration* configManager;

  Grid* gridObject;
  Axis* axisObject;
  AABB* aabbObject;
  Normal* normObject;
  Keypoint* keyObject;
  OOBB* oobbObject;
  Trajectory* trajObject;
  Localmap* mapObject;
  Mark* markObject;
  Car* carObject;
  Box* boxObject;
  Tree* treeObject;
  Matching* matchObject;
};

#endif
