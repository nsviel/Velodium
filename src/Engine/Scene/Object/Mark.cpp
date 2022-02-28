#include "Mark.h"


//Constructor / destructor
Mark::Mark(){
  //---------------------------

  this->selection_frame_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

  this->create_selection_frame();

  //---------------------------
}
Mark::~Mark(){}

void Mark::create_selection_frame(){
  selection_frame = new Glyph();
  //---------------------------

  //Create glyph
  selection_frame->name = "selection";
  selection_frame->draw_width = 1;
  selection_frame->draw_type = "line";
  selection_frame->color_unique = selection_frame_color;
  selection_frame->visibility = true;
  selection_frame->permanent = true;

  for(int i=0; i<8; i++){
    selection_frame->color.push_back(selection_frame_color);
  }

  //---------------------------
}
Glyph* Mark::obj_pointsAtLocation(vector<vec3>& pos){
  Glyph* ptAtLoc = new Glyph();
  vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  //---------------------------

  //Create glyph object
  ptAtLoc->name = "points";
  ptAtLoc->draw_width = 1;
  ptAtLoc->visibility = true;
  ptAtLoc->draw_type = "point";
  ptAtLoc->permanent = false;

  //Parameters
  vector<vec4>& RGB = ptAtLoc->color;

  //Construct glyph
  for(int i=0; i<pos.size(); i++){
    RGB.push_back(color);
  }

  //---------------------------
  return ptAtLoc;
}
Glyph* Mark::obj_sphere_RGB(double r, int lats, int longs, vec3 pos, vec3 RGB_in){
  Glyph* sphere = new Glyph();
  //---------------------------

  //Create glyph object
  sphere->name = "sphere";
  sphere->draw_width = 1;
  sphere->visibility = true;
  sphere->draw_type = "quad_strip";
  sphere->permanent = false;

  //Parameters
  vector<vec3>& XYZ = sphere->location;
  vector<vec4>& RGB = sphere->color;

  //Create glyph
  int i, j;
  for(i = 0; i <= lats; i++){
    double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
    double z0  = sin(lat0);
    double zr0 =  cos(lat0);

    double lat1 = M_PI * (-0.5 + (double) i / lats);
    double z1 = sin(lat1);
    double zr1 = cos(lat1);

    for(j = 0; j <= longs; j++) {
      double lng = 2 * M_PI * (double) (j - 1) / longs;
      double x = cos(lng);
      double y = sin(lng);

      XYZ.push_back(vec3(r * x * zr0, r * y * zr0, r * z0));
      XYZ.push_back(vec3(r * x * zr1, r * y * zr1, r * z1));

      RGB.push_back(vec4(RGB_in.x, RGB_in.y, RGB_in.z, 1.0f));
      RGB.push_back(vec4(RGB_in.x, RGB_in.y, RGB_in.z, 1.0f));
    }
  }
  /*sphere->COM = fct_c

  //Set glyph at location
  Transforms transformManager;
  transformManager.make_positionning_glyph(sphere->location, sphere->COM, pos);
  this->update_glyph_location(glyph);
  ID_sphere++;*/

  //---------------------------
  return sphere;
}
void Mark::update_selection_frame(vector<vec3> xyz){
  //---------------------------

  selection_frame->location = xyz;

  //---------------------------
}
