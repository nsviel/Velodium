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
  this->create();
}
Car::~Car(){
  //---------------------------

  delete car;

  //---------------------------
}

void Car::create(){
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
  dataFile* data = objManager.Loader("../media/engine/Marks/car.obj");
  car->xyz = data->location;
  for(int i=0; i<car->xyz.size(); i++){
    car->rgb.push_back(color);
  }

  //---------------------------
}
void Car::update(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  Frame* frame = &subset->frame;
  vec3 trans_abs = subset->root;
  //---------------------------

  if(trans_abs != vec3(0, 0, 0)){
    trans_abs.z -= lidar_height;
    transformManager->make_rotation_origin(car->xyz, subset->rotat);
    transformManager->make_translation(car->xyz, trans_abs);
  }

  //---------------------------
}
void Car::reset(){
  //---------------------------

  car->visibility = false;

  //---------------------------
}
