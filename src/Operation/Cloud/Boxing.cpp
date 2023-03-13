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

void Boxing::compute_box_MinMax(Collection* collection, vec3 min_perc, vec3 max_perc){
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  //---------------------------

  sceneManager->update_MinMax(list_obj_init);

  //Get Z extremums
  vec3 min = list_obj_init->min;
  vec3 max = list_obj_init->max;
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
void Boxing::compute_visibility(Collection* collection){
  collection->is_boxed = true;
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    this->compute_visibility(collection, i);
  }

  //---------------------------
}
void Boxing::compute_visibility(Collection* collection, int ID){
  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  Cloud* cloud = (Cloud*)collection->get_obj(ID);
  Cloud* list_obj_init = (Cloud*)collection->get_obj_init(ID);
  //---------------------------

  cloud->xyz = list_obj_init->xyz;
  cloud->I = list_obj_init->I;
  cloud->rgb = list_obj_init->rgb;

  vector<vec3>& xyz = cloud->xyz;
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

  attribManager->make_supressPoints(cloud, idx);
  sceneManager->update_buffer_location(cloud);

  //---------------------------
}

void Boxing::supress_selected_point(Collection* collection){
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    Cloud* list_obj_init = (Cloud*)collection->get_obj_init(i);
    vector<int>& idx = cloud->selected;

    cloud->xyz = list_obj_init->xyz;
    cloud->rgb = list_obj_init->rgb;
    cloud->I = list_obj_init->I;

    if(idx.size() != 0){
      attribManager->make_supressPoints(cloud, idx);
      idx.clear();
    }

    this->compute_visibility(collection, i);
  }

  //---------------------------
}
void Boxing::stop_boxing(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  //---------------------------

  //By cloud
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(), i);
    collection->is_boxed = false;
    for(int j=0; j<collection->nb_obj; j++){
      Cloud* cloud = (Cloud*)collection->get_obj(j);
      Cloud* list_obj_init = (Cloud*)collection->get_obj_init(j);
      cloud->xyz = list_obj_init->xyz;
      cloud->rgb = list_obj_init->rgb;
      cloud->I = list_obj_init->I;
      sceneManager->update_buffer_location(cloud);
    }
  }

  Box* boxObject = objectManager->get_object_box();
  Glyph* box = boxObject->get_glyph();
  box->is_visible = false;

  //---------------------------
}
