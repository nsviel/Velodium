#include "Boxing.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Scene/Glyph/Glyphs.h"
#include "../../Scene/Glyph/Cloud/Box.h"


//Constructor / destructor
Boxing::Boxing(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->objectManager = node_scene->get_objectManager();
  this->glyphManager = node_scene->get_glyphManager();
  this->attribManager = node_ope->get_attribManager();

  this->highlightON = false;

  //---------------------------
}
Boxing::~Boxing(){}

void Boxing::compute_box_MinMax(Cloud* cloud, vec3 min_perc, vec3 max_perc){
  Subset* subset_init = cloud->get_subset_selected_init();
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  //---------------------------

  sceneManager->update_MinMax(subset_init);

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
  cloud->is_boxed = true;
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    this->compute_visibility(cloud, i);
  }

  //---------------------------
}
void Boxing::compute_visibility(Cloud* cloud, int ID){
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  Subset* subset = cloud->get_subset(ID);
  Subset* subset_init = cloud->get_subset_init(ID);
  //---------------------------

  subset->xyz = subset_init->xyz;
  subset->I = subset_init->I;
  subset->rgb = subset_init->rgb;

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
  sceneManager->update_buffer_location(subset);

  //---------------------------
}

void Boxing::supress_selected_point(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = cloud->get_subset(i);
    Subset* subset_init = cloud->get_subset_init(i);
    vector<int>& idx = subset->selected;

    subset->xyz = subset_init->xyz;
    subset->rgb = subset_init->rgb;
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
    cloud->is_boxed = false;
    for(int j=0; j<cloud->nb_subset; j++){
      Subset* subset = cloud->get_subset(j);
      Subset* subset_init = cloud->get_subset_init(j);
      subset->xyz = subset_init->xyz;
      subset->rgb = subset_init->rgb;
      subset->I = subset_init->I;
      sceneManager->update_buffer_location(subset);
    }
  }

  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  box->is_visible = false;

  //---------------------------
}
