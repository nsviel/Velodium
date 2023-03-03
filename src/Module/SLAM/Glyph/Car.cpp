#include "Car.h"

#include "../Base/SLAM_utility.h"
#include "../../../Load/Format/file_OBJ.h"


Car::Car(){
  //---------------------------

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
  glyph->name = "car";
  glyph->draw_line_width = width;
  glyph->visibility = false;
  glyph->draw_type_name = "triangle";
  glyph->permanent = true;
  glyph->color_unique = color;

  //Load car model
  file_OBJ objManager;
  Data_file* data = objManager.Loader("../media/engine/Marks/car.obj");
  glyph->xyz = data->xyz;
  glyph->xyz_init = data->xyz;
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
  SLAM_utility::make_rotation_origin(glyph->xyz, subset->rotat);
  SLAM_utility::make_translation(glyph->xyz, trans_abs);

  //---------------------------
}
void Car::reset_glyph(){
  //---------------------------

  glyph->visibility = false;

  //---------------------------
}
