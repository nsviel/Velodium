#include "Car.h"

#include "../../../../Load/Format/file_OBJ.h"
#include "../../../../Operation/Transformation/Transformation.h"
#include "../../../../Specific/fct_transtypage.h"


//Constructor / destructor
Car::Car(){
  //---------------------------

  this->transformManager = new Transformation();

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = false;
  this->width = 2;
  this->lidar_height = 1.3;

  //---------------------------
  this->create_car();
}
Car::~Car(){
  //---------------------------

  delete car;

  //---------------------------
}

void Car::create_car(){
  this->car = new Glyph();
  //---------------------------

  //Create glyph
  car->name = "Car";
  car->draw_width = width;
  car->visibility = false;
  car->draw_type = "triangle";
  car->permanent = true;
  car->color_unique = color;

  //Load car model
  file_OBJ objManager;
  Data_file* data = objManager.Loader("../media/engine/Marks/car.obj");
  car->xyz = data->location;
  car->xyz_init = data->location;
  for(int i=0; i<car->xyz.size(); i++){
    car->rgb.push_back(color);
  }

  //---------------------------
}
void Car::update_car_location(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  vec3 trans_abs = subset->root;
  //---------------------------

  car->xyz = car->xyz_init;
  trans_abs.z -= lidar_height;
  transformManager->make_rotation_origin(car->xyz, subset->rotat);
  transformManager->make_translation(car->xyz, trans_abs);

  //---------------------------
}
void Car::reset(){
  //---------------------------

  car->visibility = false;

  //---------------------------
}
