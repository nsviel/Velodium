#ifndef GLYPHS_H
#define GLYPHS_H

/**
 * \file Glyphs.h
 * \brief Glyph management
 * Deal with specific background objects (grid, axis ...)
 */

#include "../common.h"

class Scene;


class Glyphs
{
public:
  //Constructor / Destructor
  Glyphs();
  ~Glyphs();

public:
  void init();
  void drawing();
  void drawing(Subset* subset);
  void reset();
  void reset_colors();

  //Glyph update
  void update(Cloud* cloud);
  void update(Subset* subset);
  void update_glyph(string name);
  void update_glyph(string name, vector<vec3> new_loc);
  void update_glyph_object(string name, Cloud* cloud);
  void update_glyph_object(string name, Subset* subset);
  void update_glyph_MinMax(Glyph* glyph);
  void update_glyph_location(Glyph* glyph);
  void update_glyph_color(Glyph* glyph);
  void update_glyph_color(Glyph* glyph, vec3 RGB);

  //Glyph creation / supression
  Glyph* create_glyph_instance(string name);
  void create_glyph_alone(Glyph& glyph);
  Glyph* create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma);
  Glyph* create_glyph_fromFile(string path);
  Glyph* create_glyph_fromFile(string path, vec3 pos, string mode, bool perma);
  Glyph* create_glyph_fromFile(string path, vec3 pos, string mode, bool perma, int size);
  void create_glyph_fromCloud(Subset* subset);
  void remove_glyph(int ID);
  void remove_glyph(string ID);

  //Setters / Getters
  void set_glyph_color(string name, vec4 RGB_new);
  void set_glyph_color(int ID, vec3 RGB);
  void set_glyph_color(Glyph* glyph, vec3 RGB);
  void set_size_normal(int size);
  void set_visibility(string name, bool value);
  void set_visibility_normal(bool value);
  void set_visibility_axisCloud(bool value);
  Glyph* get_glyph(string name);
  Glyph* get_glyph(int ID);
  vec4* get_glyph_color(string name);
  bool is_glyph_exist(string name);

private:
  Scene* sceneManager;
};

#endif
