#include "Car.h"

#include "../../../Load/Format/file_OBJ.h"
#include "../../../Operation/Transformation/Transforms.h"


//Constructor / destructor
Car::Car(){
  //---------------------------

  this->transformManager = new Transforms();
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
  car->draw_type = "quad";
  car->permanent = true;
  car->color_unique = color;

  //Load car model
  file_OBJ objManager;
  dataFile* data = objManager.Loader_complete("../media/engine/Marks/car.obj");
  car->location = data->location;
  car->location_init = data->location;
  for(int i=0; i<car->location.size(); i++){
    car->color.push_back(color);
  }

  //---------------------------
}
void Car::update(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  Frame* frame = &subset->frame;
  //---------------------------

  vec3 trans_abs = frame->trans_abs;
  car->location = car->location_init;
  transformManager->make_translation(car->location, trans_abs);

  //---------------------------
}
void Car::reset(){
  //---------------------------

  //Clear previous data
  car->location.clear();
  car->color.clear();

  //---------------------------
}
