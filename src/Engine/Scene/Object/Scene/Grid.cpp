#include "Grid.h"


//Constructor / destructor
Grid::Grid(){
  //---------------------------

  this->grid_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  this->grid_sub_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
  this->nb_cell = 4;

  this->create_grid();
  this->create_grid_sub();
  this->create_grid_plane();

  //---------------------------
}
Grid::~Grid(){}

void Grid::create_grid(){
  grid = new Glyph();
  //---------------------------

  //Create glyph
  grid->name = "grid";
  grid->draw_width = 2;
  grid->visibility = true;
  grid->draw_type = "line";
  grid->permanent = true;
  grid->color_unique = grid_color;

  //Construct grid
  this->update_grid(nb_cell);

  //---------------------------
}
void Grid::create_grid_sub(){
  grid_sub = new Glyph();
  vector<vec3>& XYZ = grid_sub->location;
  vector<vec4>& RGB = grid_sub->color;
  //---------------------------

  //Create glyph
  grid_sub->name = "grid_sub";
  grid_sub->draw_width = 2;
  grid_sub->visibility = false;
  grid_sub->draw_type = "line";
  grid_sub->permanent = true;

  //Construct grid
  this->update_grid_sub(nb_cell);

  //---------------------------
}
void Grid::create_grid_plane(){
  grid_plane = new Glyph();
  //---------------------------

  //Construct plane grid
  grid_plane->name = "grid_plane";
  grid_plane->draw_width = 1;
  grid_plane->visibility = false;
  grid_plane->draw_type = "triangle";
  grid_plane->permanent = true;

  //Parameters
  vec3 color = vec3(0.15f, 0.15f, 0.15f);
  vector<vec3>& XYZ = grid_plane->location;
  vector<vec4>& RGB = grid_plane->color;

  //Location
  XYZ.push_back(vec3(-nb_cell, -nb_cell, 0));
  XYZ.push_back(vec3(-nb_cell, nb_cell, 0));
  XYZ.push_back(vec3(nb_cell, nb_cell, 0));

  XYZ.push_back(vec3(-nb_cell, -nb_cell, 0));
  XYZ.push_back(vec3(nb_cell, -nb_cell, 0));
  XYZ.push_back(vec3(nb_cell, nb_cell, 0));

  //Color
  for(int j=0; j<6; j++){
    RGB.push_back(vec4(color.x, color.y, color.z, 1.0f));
  }

  //---------------------------
}

void Grid::update_grid(int value){
  vector<vec3>& XYZ = grid->location;
  vector<vec4>& RGB = grid->color;
  this->nb_cell = value;
  //---------------------------

  //Parameters
  XYZ.clear();
  RGB.clear();

  //Construct grid
  for(int i=-nb_cell; i<=nb_cell; i++){
    XYZ.push_back(vec3((float)i, -(float)nb_cell, 0));
    XYZ.push_back(vec3((float)i, (float)nb_cell, 0));

    XYZ.push_back(vec3(-(float)nb_cell, (float)i, 0));
    XYZ.push_back(vec3((float)nb_cell, (float)i, 0));

    for(int j=0; j<4; j++){
      RGB.push_back(grid_color);
    }
  }

  //---------------------------
}
void Grid::update_grid_sub(int value){
  vector<vec3>& XYZ = grid_sub->location;
  vector<vec4>& RGB = grid_sub->color;
  this->nb_cell = value;
  //---------------------------

  //Parameters
  int SIZE_sg = 10;
  XYZ.clear();
  RGB.clear();

  //Location
  int cpt = 0;
  for(int i=-nb_cell; i<=nb_cell-1; i++){
    for(int j=1; j<SIZE_sg; j++){
        XYZ.push_back(vec3((float)i+(float)j/SIZE_sg, (float)-nb_cell, 0));
        XYZ.push_back(vec3((float)i+(float)j/SIZE_sg, (float)nb_cell, 0));

        XYZ.push_back(vec3((float)-nb_cell, (float)i+(float)j/SIZE_sg, 0));
        XYZ.push_back(vec3((float)nb_cell, (float)i+(float)j/SIZE_sg, 0));

        cpt++;
    }
  }

  //Color
  for(int j=0; j<(cpt*4); j++){
    RGB.push_back(grid_sub_color);
  }

  //---------------------------
}
void Grid::update_grid_plane(int value){
  vector<vec3>& XYZ = grid_plane->location;
  this->nb_cell = value;
  //---------------------------

  XYZ[0] = vec3(-nb_cell, -nb_cell, 0);
  XYZ[1] = vec3(-nb_cell, nb_cell, 0);
  XYZ[2] = vec3(nb_cell, nb_cell, 0);

  XYZ[3] = vec3(-nb_cell, -nb_cell, 0);
  XYZ[4] = vec3(nb_cell, -nb_cell, 0);
  XYZ[5] = vec3(nb_cell, nb_cell, 0);

  //---------------------------
}
