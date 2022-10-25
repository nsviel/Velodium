#include "Car.h"

#include "../../../../Load/Format/file_OBJ.h"
#include "../../../../Operation/Transformation/Transforms.h"
#include "../../../../Specific/fct_transtypage.h"


//Constructor / destructor
Car::Car(){
  //---------------------------

  this->transformManager = new Transforms();
  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = false;
  this->width = 2;
  this->lidar_height = 1.3;

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
  car->visibility = false;
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

  car->visibility = visibility;

  vec3 trans_abs = subset->root;

  if(trans_abs != vec3(0, 0, 0)){
    trans_abs.z -= lidar_height;
    car->location = car->location_init;

    transformManager->make_rotation_origin(car->location, subset->rotat);
    transformManager->make_translation(car->location, trans_abs);
  }

  //---------------------------
}
void Car::reset(){
  //---------------------------

  //Clear previous data
  car->location.clear();
  car->color.clear();

  //---------------------------
}
