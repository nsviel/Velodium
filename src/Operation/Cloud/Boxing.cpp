#include "Boxing.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyph/Object.h"
#include "../../Engine/Scene/Glyph/Glyphs.h"
#include "../../Engine/Scene/Glyph/Cloud/Box.h"


//Constructor / destructor
Boxing::Boxing(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->attribManager = node_ope->get_attribManager();

  this->highlightON = false;

  //---------------------------
}
Boxing::~Boxing(){}

void Boxing::compute_box_MinMax(Cloud* cloud, vec3 min_perc, vec3 max_perc){
  Subset* subset_init = sceneManager->get_subset_init(cloud, cloud->ID_selected);
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  //---------------------------

  sceneManager->update_subset_MinMax(subset_init);

  //Get Z extremums
  vec3 min = subset_init->min;
  vec3 max = subset_init->max;
  vec3 diff = max - min;

  //Compute min abs
  vec3 min_abs;
  vec3 diff_min;
  for(int i=0; i<3; i++){
    if(min_perc[i] > 100) min_perc[i] = 100;
    if(min_perc[i] <= 0) diff_min[i] = 0;
    else diff_min[i] = diff[i] * min_perc[i]/100;

    min_abs[i] = min[i] + diff_min[i];
  }
  box->min = min_abs;

  //Compute max abs
  vec3 max_abs;
  vec3 diff_max;
  for(int i=0; i<3; i++){
    if(max_perc[i] > 100) max_perc[i] = 100;
    if(max_perc[i] <= 0) diff_max[i] = 0;
    else diff_max[i] = diff[i] * max_perc[i]/100;

    max_abs[i] = min[i] + diff_max[i];
  }
  box->max = max_abs;

  boxObject->update_box();
  glyphManager->update_glyph_location(box);
  glyphManager->update_glyph_color(box);

  //---------------------------
}
void Boxing::compute_visibility(Cloud* cloud){
  cloud->boxed = true;
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    this->compute_visibility(cloud, i);
  }

  //---------------------------
}
void Boxing::compute_visibility(Cloud* cloud, int ID){
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  Subset* subset = sceneManager->get_subset(cloud, ID);
  Subset* subset_init = sceneManager->get_subset_init(cloud, ID);
  //---------------------------

  subset->xyz = subset_init->xyz;
  subset->I = subset_init->I;
  subset->RGB = subset_init->RGB;

  vector<vec3>& xyz = subset->xyz;
  vector<int> idx;
  vec3 min = box->min;
  vec3 max = box->max;

  for(int i=0; i<xyz.size(); i++){
    if(xyz[i].x < min.x || xyz[i].x > max.x
      || xyz[i].y < min.y || xyz[i].y > max.y
      || xyz[i].z < min.z || xyz[i].z > max.z){
      idx.push_back(i);
    }
  }

  attribManager->make_supressPoints(subset, idx);
  sceneManager->update_subset_location(subset);

  //---------------------------
}

void Boxing::supress_selected_point(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Subset* subset_init = sceneManager->get_subset_init(cloud, i);
    vector<int>& idx = subset->selected;

    subset->xyz = subset_init->xyz;
    subset->RGB = subset_init->RGB;
    subset->I = subset_init->I;

    if(idx.size() != 0){
      attribManager->make_supressPoints(subset, idx);
      idx.clear();
    }

    this->compute_visibility(cloud, i);
  }

  //---------------------------
}
void Boxing::stop_boxing(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //By cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(), i);
    cloud->boxed = false;
    for(int j=0; j<cloud->nb_subset; j++){
      Subset* subset = sceneManager->get_subset(cloud, j);
      Subset* subset_init = sceneManager->get_subset_init(cloud, j);
      subset->xyz = subset_init->xyz;
      subset->RGB = subset_init->RGB;
      subset->I = subset_init->I;
      sceneManager->update_subset_location(subset);
    }
  }

  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  box->visibility = false;

  //---------------------------
}
