#include "Glyphs.h"

#include "../Node_scene.h"
#include "../Data/Scene.h"

#include "../../Load/Format/file_PTS.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Engine/GPU/GPU_data.h"


//Constructor / Destructor
Glyphs::Glyphs(Node_scene* node){
  //---------------------------

  this->gpuManager = new GPU_data();

  this->list_glyph = new list<Glyph*>;
  this->ID_glyph = 0;

  //---------------------------
}
Glyphs::~Glyphs(){
  //---------------------------

  delete list_glyph;

  //---------------------------
}

//Main functions
void Glyphs::draw_glyph(Glyph* glyph){
  //---------------------------

  if(glyph->is_visible){
    gpuManager->draw_object(glyph);
  }

  //---------------------------
}

//Glyph update
void Glyphs::update_glyph_location(Glyph* glyph){
  //---------------------------

  gpuManager->update_buffer_location(glyph);

  //---------------------------
}
void Glyphs::update_glyph_color(Glyph* glyph){
  //---------------------------

  gpuManager->update_buffer_color(glyph);

  //---------------------------
}
void Glyphs::update_glyph_color(Glyph* glyph, vec4 RGB_new){
  vector<vec4>& RGB = glyph->rgb;
  int size = RGB.size();
  //---------------------------

  //Change internal glyph color
  RGB.clear();
  for(int i=0; i<size; i++){
    RGB.push_back(RGB_new);
  }
  glyph->unicolor = RGB_new;

  //Reactualise vertex color data
  gpuManager->update_buffer_color(glyph);

  //---------------------------
}
void Glyphs::update_glyph_MinMax(Glyph* glyph){
  vector<vec3>& XYZ = glyph->xyz;
  vec3 min = XYZ[0];
  vec3 max = XYZ[0];
  vec3 centroid = vec3(0, 0, 0);
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      if ( XYZ[i][j] <= min[j] ) min[j] = XYZ[i][j];
      if ( XYZ[i][j] >= max[j] ) max[j] = XYZ[i][j];
      centroid[j] += XYZ[i][j];
    }
  }

  for(int j=0;j<3;j++){
    centroid[j] /= XYZ.size();
  }

  //---------------------------
  glyph->min = min;
  glyph->max = max;
  glyph->COM = centroid;
}

//Glyph creation / supression
void Glyphs::insert_into_gpu(Glyph* glyph){
  //---------------------------

  gpuManager->gen_vao(glyph);
  gpuManager->gen_buffer_location(glyph);
  gpuManager->gen_buffer_color(glyph);
  gpuManager->convert_draw_type_byName(glyph);

  //ID
  glyph->ID = ID_glyph++;

  //---------------------------
}
void Glyphs::remove_temporary_glyph(){
  //---------------------------

  //Remove non permanent glyphs
  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->is_permanent == false){
      this->remove_glyph_scene(glyph->ID);
      i = 0;
    }
  }

  //---------------------------
}
void Glyphs::remove_glyph_scene(int ID){
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->ID == ID){
      delete glyph;
      list<Glyph*>::iterator it = next(list_glyph->begin(), i);
      list_glyph->erase(it);
    }
  }

  //---------------------------
}
void Glyphs::create_glyph_scene(Glyph* glyph){
  //---------------------------

  this->insert_into_gpu(glyph);
  list_glyph->push_back(glyph);

  //---------------------------
}
Glyph* Glyphs::create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma){
  Glyph* glyph = new Glyph();
  unsigned int VAO;
  uint colorVBO, locationVBO;
  //---------------------------

  glyph->xyz = XYZ;
  glyph->rgb = RGB;
  glyph->name = "...";
  glyph->draw_type_name = mode;
  glyph->draw_line_width = 1;
  glyph->is_permanent = perma;

  this->insert_into_gpu(glyph);
  list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
