#ifndef GLYPHS_H
#define GLYPHS_H

#include "../../common.h"

class Engine_node;

class Grid;
class Axis;
class AABB;
class Normal;
class OOBB;
class Mark;
class Configuration;
class Trajectory;


class Glyphs
{
public:
  //Constructor / Destructor
  Glyphs(Engine_node* node_engine);
  ~Glyphs();

public:
  //Main functions
  void init_scene_object(Configuration* configManager);
  void reset_scene();
  void reset_color();

  //Runtime functions
  void runtime_glyph_scene();
  void runtime_glyph_subset(Subset* subset);
  void runtime_glyph_pred(Cloud* cloud, int subset_ID);

  //Glyph update
  void update_glyph_subset(Subset* subset);
  void update_glyph_cloud(Cloud* cloud);
  void update_glyph_location(Glyph* glyph);
  void update_glyph_color(Glyph* glyph);
  void update_glyph_color(Glyph* glyph, vec4 RGB_in);
  void update_glyph_MinMax(Glyph* glyph);

  //Glyph creation / supression
  void insert_into_gpu(Glyph* glyph);
  void remove_glyph_scene(int ID);
  void remove_glyph_subset(Subset* subset);
  void create_glyph_scene(Glyph* glyph);
  void create_glyph_subset(Subset* subset);
  Glyph* create_glyph_ostacle();
  Glyph* create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma);

  inline Grid* get_object_grid(){return gridObject;}
  inline Axis* get_object_axis(){return axisObject;}
  inline AABB* get_object_aabb(){return aabbObject;}
  inline Normal* get_object_normal(){return normObject;}
  inline Mark* get_object_mark(){return markObject;}
  inline Trajectory* get_object_trajectory(){return trajObject;}

private:
  Engine_node* node_engine;
  Grid* gridObject;
  Axis* axisObject;
  AABB* aabbObject;
  Normal* normObject;
  OOBB* oobbObject;
  Trajectory* trajObject;
  Mark* markObject;

  list<Glyph*>* list_glyph;
  bool is_visualization;
  int ID_glyph;
};

#endif
