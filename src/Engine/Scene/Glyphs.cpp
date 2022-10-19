#include "Glyphs.h"

#include "Scene.h"

#include "../Node_engine.h"
#include "../Scene/Configuration.h"

#include "../../Load/Format/file_PTS.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Glyphs::Glyphs(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();

  this->list_glyph = new list<Glyph*>;
  this->is_visualization = configManager->parse_json_b("window", "visualization");
  this->ID_glyph = 0;

  //---------------------------
}
Glyphs::~Glyphs(){}

//Main functions
void Glyphs::draw_glyph(Glyph* glyph){
  //---------------------------

  if(glyph->visibility){
    // Bind the glyph VAO
    glBindVertexArray(glyph->VAO);
    if(glyph->draw_type == "point"){
      glPointSize(glyph->draw_size);
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
    else if(glyph->draw_type == "quad"){
      glDrawArrays(GL_QUADS, 0, glyph->location.size());
    }
    else if(glyph->draw_type == "quad_strip"){
      glDrawArrays(GL_QUAD_STRIP, 0, glyph->location.size());
    }
    else if(glyph->draw_type == "polygon"){
      glDrawArrays(GL_POLYGON, 0, glyph->location.size());
    }
    else{
      glDrawArrays(GL_POINTS, 0, glyph->location.size());
    }
    glBindVertexArray(0);
  }

  //---------------------------
}

//Glyph update
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
  int size = RGB.size();
  //---------------------------

  //Change internal glyph color
  RGB.clear();
  for(int i=0; i<size; i++){
    RGB.push_back(RGB_new);
  }
  glyph->color_unique = RGB_new;

  //Reactualise vertex color data
  glBindBuffer(GL_ARRAY_BUFFER, glyph->VBO_color);
  glBufferData(GL_ARRAY_BUFFER, RGB.size() * sizeof(glm::vec4), &RGB[0],  GL_DYNAMIC_DRAW);

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

  if(is_visualization){
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
  }

  glyph->ID = ID_glyph++;

  //---------------------------
}
void Glyphs::remove_temporary_glyph(){
  //---------------------------

  //Remove non permanent glyphs
  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(),i);

    if(glyph->permanent == false){
      this->remove_glyph_scene(glyph->ID);
      i = 0;
    }
  }

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
