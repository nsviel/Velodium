#include "Car.h"

#include "../../../Load/Format/file_OBJ.h"


//Constructor / destructor
Car::Car(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->width = 2;

  //---------------------------
  this->create();
}
Car::~Car(){}

void Car::create(){
  this->car = new Glyph();
  //---------------------------

  //Create glyph
  car->name = "Car";
  car->draw_width = width;
  car->visibility = true;
  car->draw_type = "quad_strip";
  car->permanent = true;
  car->color_unique = color;

  //Load car model
  file_OBJ objManager;
  dataFile* data = objManager.Loader("../media/engine/Marks/car.obj");
  car->location = data->location;
  car->location_init = data->location;
  for(int i=0; i<car->location.size(); i++){
    car->color.push_back(color);
  }

  //---------------------------
}
void Car::update(Cloud*cloud){
  //---------------------------


  //---------------------------
}
void Car::reset(){
  //---------------------------

  //Clear previous data
  car->location.clear();
  car->color.clear();

  //---------------------------
}
