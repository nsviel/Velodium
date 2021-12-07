#include "Glyphs.h"

#include "Object/Grid.h"
#include "Object/Axis.h"
#include "Object/AABB.h"
#include "Object/Mark.h"
#include "Object/Normal.h"

#include "../Load/Format/file_PTS.h"
#include "../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Glyphs::Glyphs(){
  //---------------------------


  //---------------------------
}
Glyphs::~Glyphs(){}

//Main functions
void Glyphs::init(){
  //---------------------------

  //Grid objects
  Grid gridManager;
  Glyph* grid = gridManager.obj_grid();
  Glyph* subgrid = gridManager.obj_subgrid();
  Glyph* planegrid = gridManager.obj_planegrid();

  this->create_glyph(grid);
  this->create_glyph(subgrid);
  this->create_glyph(planegrid);

  //Axis object
  Axis axisManager;
  Glyph* axis = axisManager.obj_axis();
  this->create_glyph(axis);

  //AABB stuff
  AABB aabbManager;
  Glyph* aabb = aabbManager.obj_aabb();
  this->create_glyph(aabb);

  //Misc mark stuff
  Mark markManager;
  Glyph* selection = markManager.obj_frameSelection();
  this->create_glyph(selection);

  //Obstacle test AABB
  /*Glyph* obstacle = aabbManager.obj_obstacle();
  this->create_glyph(obstacle);
  Transforms transformManager;
  vec3 radian = transformManager.fct_degreeToRadian(vec3(50,80,0));
  mat4 transf = transformManager.compute_transformMatrix(0.5,0.5,0.5, radian.x,radian.y,radian.z);
  aabbManager.update_obstacle(obstacle, transf);
  this->update_glyph_location(obstacle);*/

  //---------------------------
}
void Glyphs::drawing(){
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->visibility){
      glBindVertexArray(glyph->VAO);
      if(glyph->draw_type == "point"){
        glPointSize(glyph->draw_width);
        glDrawArrays(GL_POINTS, 0, glyph->location.size());
      }
      else if(glyph->draw_type == "line"){ if(glyph->name=="subset_axis") say("il est bien la!");
        glLineWidth(glyph->draw_width);
        glDrawArrays(GL_LINES, 0, glyph->location.size());
        glLineWidth(1);
      }
      else if(glyph->draw_type == "triangle"){
        glDrawArrays(GL_TRIANGLES, 0, glyph->location.size());
      }
      else if(glyph->draw_type == "quad_strip"){
        glDrawArrays(GL_QUAD_STRIP, 0, glyph->location.size());
      }
      else{
        glDrawArrays(GL_POINTS, 0, glyph->location.size());
      }
      glBindVertexArray(0);
    }
  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
void Glyphs::drawing(Subset* subset){
  //---------------------------

  if(subset->visibility){
    //Subset axis
    Glyph* axis = &subset->axis;
    if(axis->visibility){
      glBindVertexArray(axis->VAO);
      glLineWidth(axis->draw_width);
      glDrawArrays(GL_LINES, 0, axis->location.size());
      glLineWidth(1);
      glBindVertexArray(0);
    }

    //Normal
    Glyph* normal = &subset->normal;
    if(normal->visibility){
      glBindVertexArray(normal->VAO);
      glLineWidth(normal->draw_width);
      glDrawArrays(GL_LINES, 0, normal->location.size());
      glLineWidth(1);
      glBindVertexArray(0);
    }

  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
void Glyphs::reset(){
  //---------------------------

  //Remove non permanent glyphs
  for(int i=0; i<database.list_glyph->size(); i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->permanent == false){
      this->remove_glyph(glyph->ID);
      i = 0;
    }
  }

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = get_glyph("aabb");
  aabb->location.clear();
  this->update_glyph_location(aabb);

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    this->update(cloud);
  }

  //---------------------------
}
void Glyphs::reset_colors(){
  //---------------------------

  if(is_glyph_exist("aabb")){
    vec3 aabbColor = vec3(1.0f, 1.0f, 1.0f);
    Glyph* glyph = get_glyph("aabb");
    this->update_glyph_color(glyph, aabbColor);
  }
  if(is_glyph_exist("matching")){
    vec3 matchingColor = vec3(0.8f, 0.8f, 0.8f);;
    Glyph* glyph = get_glyph("matching");
    this->update_glyph_color(glyph, matchingColor);
  }
  if(is_glyph_exist("normal")){
    vec3 normalColor = vec3(0.11f,0.35f,0.69f);
    Glyph* glyph = get_glyph("normal");
    this->update_glyph_color(glyph, normalColor);
  }

  vec3 gridColor = vec3(0.5f, 0.5f, 0.5f);
  Glyph* glyph = get_glyph("grid");
  this->update_glyph_color(glyph, gridColor);

  //---------------------------
}

//Glyph update
void Glyphs::update(Subset* subset){
  //---------------------------

  this->update_glyph_object("aabb", subset);

  Axis axisManager;
  axisManager.update_axis_cloud(subset);
  this->update_glyph_location(&subset->axis);

  Normal normalManager;
  normalManager.update_normal(subset);
  this->update_glyph_location(&subset->normal);
  this->update_glyph_color(&subset->normal);

  //---------------------------
}
void Glyphs::update(Cloud* cloud){
  //---------------------------

  this->update_glyph_object("aabb", cloud);

  for (int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];

    Axis axisManager;
    axisManager.update_axis_cloud(subset);
    this->update_glyph_location(&subset->axis);

    Normal normalManager;
    normalManager.update_normal(subset);
    this->update_glyph_location(&subset->normal);
    this->update_glyph_color(&subset->normal);
  }

  //---------------------------
}
void Glyphs::update_glyph(string name){
  //---------------------------

  for(int i=0; i<database.list_glyph->size(); i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      this->update_glyph_location(glyph);
      this->update_glyph_color(glyph);
    }
  }

  //---------------------------
}
void Glyphs::update_glyph(string name, vector<vec3> new_loc){
  //---------------------------

  for(int i=0; i<database.list_glyph->size(); i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      glyph->location = new_loc;
      this->update_glyph_location(glyph);
    }
  }

  //---------------------------
}
void Glyphs::update_glyph_object(string name, Cloud* cloud){
  Glyph* glyph = get_glyph(name);
  //---------------------------

  if(name == "aabb"){
    AABB aabbManager;
    aabbManager.update_aabb(glyph, cloud);

    this->update_glyph_location(glyph);
    this->update_glyph_color(glyph);
  }

  //---------------------------

}
void Glyphs::update_glyph_object(string name, Subset* subset){
  Glyph* glyph = get_glyph(name);
  //---------------------------

  if(name == "aabb"){
    AABB aabbManager;
    aabbManager.update_aabb(glyph, subset);

    this->update_glyph_location(glyph);
    this->update_glyph_color(glyph);
  }

  //---------------------------

}
void Glyphs::update_glyph_location(Glyph* glyph){
  vector<vec3>& XYZ = glyph->location;
  //---------------------------

  //Reactualise vertex position data
  glBindBuffer(GL_ARRAY_BUFFER, glyph->VBO_location);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void Glyphs::update_glyph_color(Glyph* glyph){
  vector<vec4>& RGB = glyph->color;
  //---------------------------

  //Reactualise vertex color data
  glBindBuffer(GL_ARRAY_BUFFER, glyph->VBO_color);
  glBufferData(GL_ARRAY_BUFFER, RGB.size() * sizeof(glm::vec4), &RGB[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void Glyphs::update_glyph_color(Glyph* glyph, vec3 RGB_in){
  vector<vec4>& RGB = glyph->color;
  //---------------------------

  for(int i=0; i<RGB.size(); i++){
    RGB[i] = vec4(RGB_in, 1.0f);
  }

  glBindVertexArray(glyph->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, glyph->VBO_color);
  glBufferData(GL_ARRAY_BUFFER, RGB.size() * sizeof(glm::vec4), &RGB[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void*)(4* sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  //---------------------------
}
void Glyphs::update_glyph_MinMax(Glyph* glyph){
  vector<vec3>& XYZ = glyph->location;
  vec3 min = XYZ[0];
  vec3 max = XYZ[0];
  vec3 centroid = vec3(0, 0, 0);
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      if ( XYZ[i][j] <= min[j] ) min[j] = XYZ[i][j];
      if ( XYZ[i][j] >= max[j] ) max[j] = XYZ[i][j];
      centroid[j] += XYZ[i][j];
    }
  }

  for(int j=0;j<3;j++){
    centroid[j] /= XYZ.size();
  }

  //---------------------------
  glyph->min = min;
  glyph->max = max;
  glyph->COM = centroid;
}

//Glyph creation / supression
Glyph* Glyphs::create_glyph(Glyph* glyph){
  vector<vec3>& XYZ = glyph->location;
  vector<vec4>& RGB = glyph->color;
  //---------------------------

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //Vertices
  GLuint locationVBO;
  glGenBuffers(1, &locationVBO);
  glBindBuffer(GL_ARRAY_BUFFER, locationVBO);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  //Color
  GLuint colorVBO;
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, RGB.size()*sizeof(glm::vec4), &RGB[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glyph->VAO = VAO;
  glyph->VBO_location = locationVBO;
  glyph->VBO_color = colorVBO;
  glyph->ID = database.ID_glyph++;

  database.list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
void Glyphs::create_glyph_alone(Glyph& glyph){
  vector<vec3>& XYZ = glyph.location;
  vector<vec4>& RGB = glyph.color;
  //---------------------------

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //Vertices
  GLuint locationVBO;
  glGenBuffers(1, &locationVBO);
  glBindBuffer(GL_ARRAY_BUFFER, locationVBO);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  //Color
  GLuint colorVBO;
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, RGB.size()*sizeof(glm::vec4), &RGB[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glyph.VAO = VAO;
  glyph.VBO_location = locationVBO;
  glyph.VBO_color = colorVBO;
  glyph.ID = database.ID_glyph++;

  //---------------------------
}
Glyph* Glyphs::create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma){
  Glyph* glyph = new Glyph();
  unsigned int VAO;
  uint colorVBO, locationVBO;
  //---------------------------

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //Vertices
  glGenBuffers(1, &locationVBO);
  glBindBuffer(GL_ARRAY_BUFFER, locationVBO);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

  //Color
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, RGB.size() * sizeof(glm::vec4), &RGB[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,4 * sizeof(float),(void*)(4* sizeof(float)));
  glEnableVertexAttribArray(1);

  glyph->VAO = VAO;
  glyph->VBO_location = locationVBO;
  glyph->VBO_color = colorVBO;
  glyph->location = XYZ;
  glyph->color = RGB;

  glyph->name = "...";
  glyph->draw_type = mode;
  glyph->draw_width = 1;
  glyph->ID = database.ID_glyph++;
  glyph->permanent = perma;

  database.list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph_fromFile(string filePath){
  //---------------------------

  //Load data
  filePTS ptsManager;
  dataFile* data = ptsManager.Loader(filePath);

  //Extract data
  Glyph* glyph = new Glyph();

  uint VAO, colorVBO, locationVBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &locationVBO);
  glGenBuffers(1, &colorVBO);
  glyph->VAO = VAO;
  glyph->VBO_location = locationVBO;
  glyph->VBO_color = colorVBO;
  glyph->location = data->location;
  glyph->color = data->color;

  string nameFormat = filePath.substr(filePath.find_last_of("/\\") + 1);
  glyph->name =  nameFormat.substr(0, nameFormat.find_last_of("."));
  glyph->draw_type = "point";
  glyph->draw_width = 1;
  glyph->visibility = true;

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph_fromFile(string path, vec3 pos, string mode, bool perma){
  Glyph* glyph = create_glyph_fromFile(path);
  //---------------------------

  glyph->ID = database.ID_glyph++;
  glyph->draw_type = mode;
  glyph->permanent = perma;

  this->update_glyph_MinMax(glyph);

  //Set glyph at location
  Transforms transformManager;
  transformManager.make_positionning_glyph(glyph->location, glyph->COM, pos);
  this->update_glyph_MinMax(glyph);
  this->update_glyph_location(glyph);
  database.list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph_fromFile(string path, vec3 pos, string mode, bool perma, int point_size){
  Glyph* glyph = create_glyph_fromFile(path);
  //---------------------------

  glyph->ID = database.ID_glyph++;
  glyph->draw_type = mode;
  glyph->draw_width = point_size;
  glyph->permanent = perma;

  this->update_glyph_MinMax(glyph);

  //Set glyph at location
  Transforms transformManager;
  transformManager.make_positionning_glyph(glyph->location, glyph->COM, pos);
  this->update_glyph_MinMax(glyph);
  this->update_glyph_location(glyph);
  database.list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
void Glyphs::create_glyph_fromCloud(Subset* subset){
  //---------------------------

  //Cloud axis stuff
  Axis axisManager;
  axisManager.obj_axis_cloud(subset);
  this->create_glyph_alone(subset->axis);

  //Normal stuff
  Normal normalManager;
  normalManager.obj_normal(subset);
  this->create_glyph_alone(subset->normal);

  //---------------------------
}
void Glyphs::remove_glyph(int ID){
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->ID == ID){
      delete glyph;
      list<Glyph*>::iterator it = next(database.list_glyph->begin(), i);
      database.list_glyph->erase(it);
    }
  }

  //---------------------------
}
void Glyphs::remove_glyph(string ID){
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == ID){
      delete glyph;
      list<Glyph*>::iterator it = next(database.list_glyph->begin(), i);
      database.list_glyph->erase(it);
    }
  }

  //---------------------------
}

//Setters / Getters
void Glyphs::set_glyph_color(int ID, vec3 RGB_new){
  Glyph* glyph = get_glyph(ID);
  //---------------------------

  //Reactualise color data
  vector<vec4>& RGB = glyph->color;
  RGB.clear();
  for(int i=0; i<glyph->location.size(); i++){
    RGB.push_back(vec4(RGB_new, 1.0f));
  }
  glyph->color_unique = vec4(RGB_new, 1.0f);

  //---------------------------
  this->update_glyph_color(glyph);
}
void Glyphs::set_glyph_color(string name, vec4 RGB_new){
  Glyph* glyph = get_glyph(name);
  //---------------------------

  //Reactualise color data
  vector<vec4>& RGB = glyph->color;
  RGB.clear();
  for(int i=0; i<glyph->location.size(); i++){
    RGB.push_back(RGB_new);
  }
  glyph->color_unique = RGB_new;

  //---------------------------
  this->update_glyph_color(glyph);
}
void Glyphs::set_size_normal(int size){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->normal.draw_size = size;
      this->update(subset);
    }
  }

  //---------------------------
}
void Glyphs::set_visibility(string name, bool value){
  //---------------------------

  for(int i=0; i<database.list_glyph->size(); i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      glyph->visibility = value;
    }
  }

  //---------------------------
}
void Glyphs::set_visibility_normal(bool value){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->normal.visibility = value;
    }
  }

  //---------------------------
}
void Glyphs::set_visibility_axisCloud(bool value){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->axis.visibility = value;
    }
  }

  //---------------------------
}
Glyph* Glyphs::get_glyph(string name){
  Glyph* glyph_out;
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      glyph_out = glyph;
    }
  }

  //---------------------------
  return glyph_out;
}
Glyph* Glyphs::get_glyph(int ID){
  Glyph* glyph_out;
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->ID == ID){
      glyph_out = glyph;
    }
  }

  //---------------------------
  return glyph_out;
}
vec4* Glyphs::get_glyph_color(string name){
  vec4* color;
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      color = &glyph->color_unique;
    }
  }

  //---------------------------
  return color;
}
bool Glyphs::is_glyph_exist(string name){
  bool exist = false;
  //---------------------------

  for(int i=0;i<database.list_glyph->size();i++){
    Glyph* glyph = *next(database.list_glyph->begin(),i);

    if(glyph->name == name){
      exist = true;
      break;
    }
  }

  //---------------------------
  return exist;
}
