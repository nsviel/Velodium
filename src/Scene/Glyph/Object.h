#ifndef OBJECT_H
#define OBJECT_H

#include "../../common.h"

class Node_scene;
class Node_engine;
class Configuration;
class Glyphs;

class Glyph_source;

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
  Object(Node_scene* node);
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
  Glyph_source* get_glyph_src_byName(string name);

  inline Grid* get_object_grid(){return (Grid*)get_glyph_src_byName("grid");}
  inline Axis* get_object_axis(){return (Axis*)get_glyph_src_byName("axis");}
  inline AABB* get_object_aabb(){return (AABB*)get_glyph_src_byName("aabb");}
  inline Normal* get_object_normal(){return (Normal*)get_glyph_src_byName("normal");}
  inline Mark* get_object_mark(){return (Mark*)get_glyph_src_byName("mark");}
  inline Box* get_object_box(){return (Box*)get_glyph_src_byName("box");}
  inline Tree* get_object_tree(){return (Tree*)get_glyph_src_byName("tree");}
  inline Matching* get_object_matching(){return (Matching*)get_glyph_src_byName("matching");}

  inline Trajectory* get_object_trajectory(){return trajObject;}
  inline Car* get_object_car(){return carObject;}
  inline Keypoint* get_object_keypoint(){return keyObject;}
  inline Localmap* get_object_localmap(){return mapObject;}

private:
  Node_engine* node_engine;
  Glyphs* glyphManager;
  Configuration* configManager;

  vector<Glyph_source*> vec_glyph_src;

  Trajectory* trajObject;
  Localmap* mapObject;
  Keypoint* keyObject;
  Car* carObject;
};

#endif
