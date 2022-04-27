#ifndef GLYPHS_H
#define GLYPHS_H

#include "../../common.h"

class Engine_node;


class Glyphs
{
public:
  //Constructor / Destructor
  Glyphs(Engine_node* node_engine);
  ~Glyphs();

public:
  //Main functions
  void reset_scene();
  void reset_color();
  void draw_glyph(Glyph* glyph);

  //Runtime functions
  void runtime_scene_glyph();
  void runtime_glyph_pred(Cloud* cloud, int subset_ID);

  //Glyph update
  void update_glyph_location(Glyph* glyph);
  void update_glyph_color(Glyph* glyph);
  void update_glyph_color(Glyph* glyph, vec4 RGB_in);
  void update_glyph_MinMax(Glyph* glyph);

  //Glyph creation / supression
  void insert_into_gpu(Glyph* glyph);
  void remove_glyph_scene(int ID);
  void remove_temporary_glyph();
  void create_glyph_scene(Glyph* glyph);
  Glyph* create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma);

private:
  Engine_node* node_engine;

  list<Glyph*>* list_glyph;
  bool is_visualization;
  int ID_glyph;
};

#endif
