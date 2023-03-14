#ifndef GLYPHS_H
#define GLYPHS_H

#include "../Data/Data.h"
#include "../../common.h"

class GPU_data;


class Glyphs
{
public:
  //Constructor / Destructor
  Glyphs();
  ~Glyphs();

public:
  //Main functions
  void reset_scene();
  void reset_color();
  void draw_glyph(Glyph* glyph);

  //Glyph update
  void update_glyph_buffer(Glyph* glyph);
  void update_glyph_location(Glyph* glyph);
  void update_glyph_color(Glyph* glyph);
  void update_glyph_color(Glyph* glyph, vec4 RGB_in);
  void update_glyph_MinMax(Glyph* glyph);

  //Glyph creation / supression
  void insert_into_gpu(Glyph* glyph);
  void remove_glyph_scene(int ID);
  void remove_temporary_glyph();
  void create_glyph_scene(Glyph* glyph);
  void create_glyph_object(Cloud* cloud, Glyph* glyph);
  Glyph* create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma);

private:
  GPU_data* gpuManager;
  Data* data;

  list<Glyph*>* list_glyph;
  int ID_glyph;
};

#endif
