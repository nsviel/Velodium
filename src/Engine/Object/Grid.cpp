#include "Grid.h"


//Constructor / destructor
Grid::Grid(){
  //---------------------------

  this->grid_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  this->nb_cell = 4;

  //---------------------------
}
Grid::~Grid(){}

Glyph* Grid::obj_grid(){
  Glyph* grid = new Glyph();
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
  return grid;
}
Glyph* Grid::obj_subgrid(){
  Glyph* subgrid = new Glyph();
  vector<vec3>& XYZ = subgrid->location;
  vector<vec4>& RGB = subgrid->color;
  //---------------------------

  //Create glyph
  subgrid->name = "subgrid";
  subgrid->draw_width = 1;
  subgrid->visibility = false;
  subgrid->draw_type = "line";
  subgrid->permanent = true;

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
  return subgrid;
}
Glyph* Grid::obj_planegrid(){
  Glyph* planegrid = new Glyph();
  //---------------------------

  //Construct plane grid
  planegrid->name = "planegrid";
  planegrid->draw_width = 1;
  planegrid->visibility = false;
  planegrid->draw_type = "triangle";
  planegrid->permanent = true;

  //Parameters
  vec3 color = vec3(0.15f, 0.15f, 0.15f);
  int SIZE = nb_cell;
  vector<vec3>& XYZ = planegrid->location;
  vector<vec4>& RGB = planegrid->color;

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
  return planegrid;
}
