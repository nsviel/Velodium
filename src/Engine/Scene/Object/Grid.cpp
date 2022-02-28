#include "Grid.h"


//Constructor / destructor
Grid::Grid(){
  //---------------------------

  this->grid_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
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
  grid->draw_width = 1;
  grid->visibility = true;
  grid->draw_type = "line";
  grid->permanent = true;
  grid->color_unique = grid_color;

  //Grid parameters
  int SIZE = nb_cell;
  vector<vec3>& XYZ = grid->location;
  vector<vec4>& RGB = grid->color;

  //Construct grid
  for(int i=-SIZE; i<=SIZE; i++){
    XYZ.push_back(vec3((float)i, -(float)SIZE, 0));
    XYZ.push_back(vec3((float)i, (float)SIZE, 0));

    XYZ.push_back(vec3(-(float)SIZE, (float)i, 0));
    XYZ.push_back(vec3((float)SIZE, (float)i, 0));

    for(int j=0; j<4; j++){
      RGB.push_back(grid_color);
    }
  }

  //---------------------------
}
void Grid::create_grid_sub(){
  grid_sub = new Glyph();
  vector<vec3>& XYZ = grid_sub->location;
  vector<vec4>& RGB = grid_sub->color;
  //---------------------------

  //Create glyph
  grid_sub->name = "grid_sub";
  grid_sub->draw_width = 1;
  grid_sub->visibility = false;
  grid_sub->draw_type = "line";
  grid_sub->permanent = true;

  //Parameters
  int SIZE_sg = 10;
  int SIZE = nb_cell;
  vec3 color = vec3(0.2f, 0.2f, 0.2f);

  //Location
  int cpt = 0;
  for(int i=-SIZE; i<=SIZE-1; i++){
    for(int j=1; j<SIZE_sg; j++){
        XYZ.push_back(vec3((float)i+(float)j/SIZE_sg, (float)-SIZE, 0));
        XYZ.push_back(vec3((float)i+(float)j/SIZE_sg, (float)SIZE, 0));

        XYZ.push_back(vec3((float)-SIZE, (float)i+(float)j/SIZE_sg, 0));
        XYZ.push_back(vec3((float)SIZE, (float)i+(float)j/SIZE_sg, 0));

        cpt++;
    }
  }

  //Color
  for(int j=0; j<(cpt*4); j++){
    RGB.push_back(vec4(color.x, color.y, color.z, 1.0f));
  }

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
  int SIZE = nb_cell;
  vector<vec3>& XYZ = grid_plane->location;
  vector<vec4>& RGB = grid_plane->color;

  //Create plane
  //Location
  XYZ.push_back(vec3(-SIZE, -SIZE, 0));
  XYZ.push_back(vec3(-SIZE, SIZE, 0));
  XYZ.push_back(vec3(SIZE, SIZE, 0));

  XYZ.push_back(vec3(-SIZE, -SIZE, 0));
  XYZ.push_back(vec3(SIZE, -SIZE, 0));
  XYZ.push_back(vec3(SIZE, SIZE, 0));

  //Color
  for(int j=0; j<6; j++){
    RGB.push_back(vec4(color.x, color.y, color.z, 1.0f));
  }

  //---------------------------
}
