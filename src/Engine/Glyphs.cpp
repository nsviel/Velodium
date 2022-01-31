#include "Glyphs.h"

#include "Scene.h"

#include "Object/Grid.h"
#include "Object/Axis.h"
#include "Object/AABB.h"
#include "Object/OOBB.h"
#include "Object/Mark.h"
#include "Object/Normal.h"

#include "../Load/Format/file_PTS.h"
#include "../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Glyphs::Glyphs(){
  //---------------------------

  Scene* sceneManager = new Scene();

  //---------------------------
}
Glyphs::~Glyphs(){}

//Main functions
void Glyphs::init(){
  //---------------------------

  this->create_glyph_instance("grid");
  this->create_glyph_instance("subgrid");
  this->create_glyph_instance("planegrid");
  this->create_glyph_instance("axis");
  this->create_glyph_instance("aabb");
  this->create_glyph_instance("selection");

  //---------------------------
}
void Glyphs::drawing(){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

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

    //OOBB - ground thruth
    vector<Glyph*>& oobb_gt = subset->obstacle_gt.oobb;
    for(int i=0; i<oobb_gt.size(); i++){
      glBindVertexArray(oobb_gt[i]->VAO);
      glLineWidth(oobb_gt[i]->draw_width);
      glDrawArrays(GL_LINES, 0, oobb_gt[i]->location.size());
      glLineWidth(1);
      glBindVertexArray(0);
    }

    //OOBB - prediction
    vector<Glyph*>& oobb_pr = subset->obstacle_pr.oobb;
    for(int i=0; i<oobb_pr.size(); i++){
      glBindVertexArray(oobb_pr[i]->VAO);
      glLineWidth(oobb_pr[i]->draw_width);
      glDrawArrays(GL_LINES, 0, oobb_pr[i]->location.size());
      glLineWidth(1);
      glBindVertexArray(0);
    }
  }

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
void Glyphs::reset(){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Remove non permanent glyphs
  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->permanent == false){
      this->remove_glyph(glyph->ID);
      i = 0;
    }
  }

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = get_glyph("aabb");
  aabb->location.clear();
  this->update_glyph_location(aabb);

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
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

  //this->update_glyph_object("aabb", subset);

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
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == name){
      this->update_glyph_location(glyph);
      this->update_glyph_color(glyph);
    }
  }

  //---------------------------
}
void Glyphs::update_glyph(string name, vector<vec3> new_loc){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

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
Glyph* Glyphs::create_glyph_instance(string name){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  int* ID_glyph = sceneManager->get_list_ID_glyph();
  Glyph* glyph;
  //---------------------------

  if(name == "grid"){
    Grid gridManager;
    glyph = gridManager.obj_grid();
  }
  if(name == "subgrid"){
    Grid gridManager;
    glyph = gridManager.obj_subgrid();
  }
  if(name == "planegrid"){
    Grid gridManager;
    glyph = gridManager.obj_planegrid();
  }
  if(name == "axis"){
    Axis axisManager;
    glyph = axisManager.obj_axis();
  }
  if(name == "aabb"){
    AABB aabbManager;
    glyph = aabbManager.obj_aabb();
  }
  if(name == "obstacle"){
    OOBB oobbManager;
    glyph = oobbManager.obj_oobb();
  }
  if(name == "selection"){
    Mark markManager;
    glyph = markManager.obj_frameSelection();
  }

  //OpenGL stuff
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //Vertices
  vector<vec3>& XYZ = glyph->location;
  GLuint locationVBO;
  glGenBuffers(1, &locationVBO);
  glBindBuffer(GL_ARRAY_BUFFER, locationVBO);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  //Color
  vector<vec4>& RGB = glyph->color;
  GLuint colorVBO;
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, RGB.size()*sizeof(glm::vec4), &RGB[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glyph->VAO = VAO;
  glyph->VBO_location = locationVBO;
  glyph->VBO_color = colorVBO;
  glyph->ID = *ID_glyph++;

  list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
void Glyphs::create_glyph_alone(Glyph& glyph){
  int* ID_glyph = sceneManager->get_list_ID_glyph();
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
  glyph.ID = *ID_glyph++;

  //---------------------------
}
Glyph* Glyphs::create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  Glyph* glyph = new Glyph();
  int* ID_glyph = sceneManager->get_list_ID_glyph();
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
  glyph->ID = *ID_glyph++;
  glyph->permanent = perma;

  list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph_fromFile(string filePath){
  //---------------------------

  //Load data
  file_PTS ptsManager;
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
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  Glyph* glyph = create_glyph_fromFile(path);
  int* ID_glyph = sceneManager->get_list_ID_glyph();
  //---------------------------

  glyph->ID = *ID_glyph++;
  glyph->draw_type = mode;
  glyph->permanent = perma;

  this->update_glyph_MinMax(glyph);

  //Set glyph at location
  Transforms transformManager;
  transformManager.make_positionning_glyph(glyph->location, glyph->COM, pos);
  this->update_glyph_MinMax(glyph);
  this->update_glyph_location(glyph);
  list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph_fromFile(string path, vec3 pos, string mode, bool perma, int point_size){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  Glyph* glyph = create_glyph_fromFile(path);
  int* ID_glyph = sceneManager->get_list_ID_glyph();
  //---------------------------

  glyph->ID = *ID_glyph++;
  glyph->draw_type = mode;
  glyph->draw_width = point_size;
  glyph->permanent = perma;

  this->update_glyph_MinMax(glyph);

  //Set glyph at location
  Transforms transformManager;
  transformManager.make_positionning_glyph(glyph->location, glyph->COM, pos);
  this->update_glyph_MinMax(glyph);
  this->update_glyph_location(glyph);
  list_glyph->push_back(glyph);

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
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->ID == ID){
      delete glyph;
      list<Glyph*>::iterator it = next(list_glyph->begin(), i);
      list_glyph->erase(it);
    }
  }

  //---------------------------
}
void Glyphs::remove_glyph(string ID){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == ID){
      delete glyph;
      list<Glyph*>::iterator it = next(list_glyph->begin(), i);
      list_glyph->erase(it);
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
void Glyphs::set_glyph_color(Glyph* glyph, vec3 RGB_new){
  vector<vec4>& RGB = glyph->color;
  int size = RGB.size();
  //---------------------------

  RGB.clear();

  //Reactualise color data
  for(int i=0; i<size; i++){
    RGB.push_back(vec4(RGB_new, 1.0f));
  }
  glyph->color_unique = vec4(RGB_new, 1.0f);

  //---------------------------
  this->update_glyph_color(glyph);
}
void Glyphs::set_size_normal(int size){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->normal.draw_size = size;
      this->update(subset);
    }
  }

  //---------------------------
}
void Glyphs::set_visibility(string name, bool value){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  //---------------------------

  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == name){
      glyph->visibility = value;
    }
  }

  //---------------------------
}
void Glyphs::set_visibility_normal(bool value){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->normal.visibility = value;
    }
  }

  //---------------------------
}
void Glyphs::set_visibility_axisCloud(bool value){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    for (int j=0; j<cloud->nb_subset; j++){
      Subset* subset = &cloud->subset[j];

      subset->axis.visibility = value;
    }
  }

  //---------------------------
}
Glyph* Glyphs::get_glyph(string name){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  Glyph* glyph_out;
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == name){
      glyph_out = glyph;
    }
  }

  //---------------------------
  return glyph_out;
}
Glyph* Glyphs::get_glyph(int ID){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  Glyph* glyph_out;
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->ID == ID){
      glyph_out = glyph;
    }
  }

  //---------------------------
  return glyph_out;
}
vec4* Glyphs::get_glyph_color(string name){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  vec4* color;
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == name){
      color = &glyph->color_unique;
    }
  }

  //---------------------------
  return color;
}
bool Glyphs::is_glyph_exist(string name){
  list<Glyph*>* list_glyph = sceneManager->get_list_glyph();
  bool exist = false;
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->name == name){
      exist = true;
      break;
    }
  }

  //---------------------------
  return exist;
}
