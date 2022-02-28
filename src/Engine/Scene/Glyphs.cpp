#include "Glyphs.h"

#include "Scene.h"

#include "Object/Grid.h"
#include "Object/Axis.h"
#include "Object/AABB.h"
#include "Object/OOBB.h"
#include "Object/Mark.h"
#include "Object/Normal.h"

#include "../Engine_node.h"

#include "../../Load/Format/file_PTS.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Glyphs::Glyphs(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->list_glyph = new list<Glyph*>;
  this->gridObject = new Grid();
  this->axisObject = new Axis();
  this->aabbObject = new AABB();
  this->normObject = new Normal();
  this->oobbObject = new OOBB();

  this->ID_glyph = 0;

  //---------------------------
  this->init_scene_object();
}
Glyphs::~Glyphs(){}

//Main functions
void Glyphs::init_scene_object(){
  //---------------------------

  this->create_glyph_scene(gridObject->get_grid());
  this->create_glyph_scene(gridObject->get_grid_sub());
  this->create_glyph_scene(gridObject->get_grid_plane());
  this->create_glyph_scene(axisObject->get_axis_scene());
  this->create_glyph_scene(aabbObject->get_aabb());

  //---------------------------
}
void Glyphs::reset_scene(){
  //---------------------------

  //Remove non permanent glyphs
  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->permanent == false){
      this->remove_glyph_scene(glyph->ID);
      i = 0;
    }
  }

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = aabbObject->get_aabb();
  if(aabb != nullptr){
    aabb->location.clear();
    this->update_glyph_location(aabb);
  }

  //---------------------------
}
void Glyphs::reset_color(){
  //---------------------------

  Glyph* aabb = aabbObject->get_aabb();
  Glyph* grid = gridObject->get_grid();

  this->update_glyph_color(aabb, vec4(1.0f, 1.0f, 1.0f, 1.0f));
  this->update_glyph_color(grid, vec4(0.5f, 0.5f, 0.5f, 1.0f));

  //---------------------------
}

//Runtime functions
void Glyphs::runtime_glyph_scene(){
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->visibility){
      // Bind the glyph VAO
      glBindVertexArray(glyph->VAO);
      if(glyph->draw_type == "point"){
        glPointSize(glyph->draw_width);
        glDrawArrays(GL_POINTS, 0, glyph->location.size());
      }
      else if(glyph->draw_type == "line"){
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
void Glyphs::runtime_glyph_subset(Subset* subset){
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
void Glyphs::runtime_glyph_pred(Subset* subset){
  //---------------------------

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

  //---------------------------
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

//Glyph update
void Glyphs::update_glyph_subset(Subset* subset){
  //---------------------------

  //Subset axis
  axisObject->update_axis_subset(subset);
  this->update_glyph_location(&subset->axis);

  //Subset normal
  normObject->update_normal_subset(subset);
  this->update_glyph_location(&subset->normal);
  this->update_glyph_color(&subset->normal);

  //---------------------------
}
void Glyphs::update_glyph_cloud(Cloud* cloud){
  //---------------------------

  //Update cloud AABB
  aabbObject->update_aabb(cloud);
  this->update_glyph_location(aabbObject->get_aabb());
  this->update_glyph_color(aabbObject->get_aabb());

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
void Glyphs::update_glyph_color(Glyph* glyph, vec4 RGB_new){
  vector<vec4>& RGB = glyph->color;
  //---------------------------

  //Change internal glyph color
  RGB.clear();
  for(int i=0; i<RGB.size(); i++){
    RGB[i] = RGB_new;
  }
  glyph->color_unique = RGB_new;

  //Update gpu
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
void Glyphs::insert_into_gpu(Glyph* glyph){
  //---------------------------

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
  glyph->ID = ID_glyph++;

  //---------------------------
}
void Glyphs::remove_glyph_scene(int ID){
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
void Glyphs::create_glyph_scene(Glyph* glyph){
  //---------------------------

  this->insert_into_gpu(glyph);
  list_glyph->push_back(glyph);

  //---------------------------
}
void Glyphs::create_glyph_subset(Subset* subset){
  //---------------------------

  //Cloud axis stuff
  axisObject->create_axis_subset(subset);
  this->insert_into_gpu(&subset->axis);

  //Normal stuff
  normObject->create_normal_subset(subset);
  this->insert_into_gpu(&subset->normal);

  //---------------------------
}
Glyph* Glyphs::create_glyph_ostacle(){
  //---------------------------

  //Creat new OOBB object
  oobbObject->create_oobb();
  Glyph* glyph = oobbObject->get_oobb();
  this->insert_into_gpu(glyph);

  //---------------------------
  return glyph;
}
Glyph* Glyphs::create_glyph(vector<vec3>& XYZ, vector<vec4>& RGB, string mode, bool perma){
  Glyph* glyph = new Glyph();
  unsigned int VAO;
  uint colorVBO, locationVBO;
  //---------------------------

  glyph->location = XYZ;
  glyph->color = RGB;
  glyph->name = "...";
  glyph->draw_type = mode;
  glyph->draw_width = 1;
  glyph->permanent = perma;

  this->insert_into_gpu(glyph);
  list_glyph->push_back(glyph);

  //---------------------------
  return glyph;
}
