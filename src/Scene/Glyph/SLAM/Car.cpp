#include "Car.h"

#include "../../../Load/Format/file_OBJ.h"
#include "../../../Operation/Transformation/Transformation.h"
#include "../../../Specific/Function/fct_transtypage.h"


Car::Car(){
  //---------------------------

  this->transformManager = new Transformation();

  this->visibility = false;
  this->width = 2;
  this->lidar_height = 1.3;

  //---------------------------
  this->create_glyph();
}

void Car::create_glyph(){
  this->glyph = new Glyph();
  //---------------------------

  //Create glyph
  glyph->name = "Car";
  glyph->draw_width = width;
  glyph->visibility = false;
  glyph->draw_type = "triangle";
  glyph->permanent = true;
  glyph->color_unique = color;

  //Load car model
  file_OBJ objManager;
  Data_file* data = objManager.Loader("../media/engine/Marks/car.obj");
  glyph->xyz = data->location;
  glyph->xyz_init = data->location;
  for(int i=0; i<glyph->xyz.size(); i++){
    glyph->rgb.push_back(color);
  }

  //---------------------------
}
void Car::update_glyph(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  vec3 trans_abs = subset->root;
  //---------------------------

  glyph->xyz = glyph->xyz_init;
  trans_abs.z -= lidar_height;
  transformManager->make_rotation_origin(glyph->xyz, subset->rotat);
  transformManager->make_translation(glyph->xyz, trans_abs);

  //---------------------------
}
void Car::reset_glyph(){
  //---------------------------

  glyph->visibility = false;

  //---------------------------
}
