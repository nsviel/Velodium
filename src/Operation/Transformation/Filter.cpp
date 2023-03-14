#include "Filter.h"

#include "Attribut.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/Function/fct_terminal.h"


//Constructor / Destructor
Filter::Filter(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();

  //---------------------------
  this->update_configuration();
}
Filter::~Filter(){}

void Filter::update_configuration(){
  //---------------------------

  this->verbose = false;
  this->sphere_D = 0.139f;
  this->sphere_min = configManager->parse_json_f("parameter", "filter_sphere_rmin");
  this->sphere_max = configManager->parse_json_f("parameter", "filter_sphere_rmax");
  this->cyl_r_min = configManager->parse_json_f("parameter", "filter_cylinder_rmin");
  this->cyl_r_max = configManager->parse_json_f("parameter", "filter_cylinder_rmax");
  this->cyl_z_min = -3;

  //---------------------------
}

//Functions
void Filter::filter_maxAngle(Collection* collection, float angleMax){
  Cloud* cloud = (Cloud*)collection->selected_obj;
  attribManager->compute_attribut_subset(cloud);
  vector<float>& It = cloud->It;
  int size_before = cloud->nb_point;
  tic();
  //---------------------------

  vector<int> idx;
  for(int i=0; i<It.size(); i++){
    if(It[i] >= angleMax){
      idx.push_back(i);
    }
  }

  //Supress points with angle superior to the limit
  attribManager->make_supressPoints(cloud, idx);

  //---------------------------
  float duration = toc();
  if(verbose){
    int size_filtered = cloud->xyz.size();
    string log = "Filter by angle (" + to_string(angleMax) + "°) : " + to_string(size_before) + " -> " + to_string(size_filtered) + " points (" + to_string(duration) + " ms)";
    console.AddLog("ok", log);
  }
}
void Filter::filter_sphere(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  float r = sphere_D/2;
  float err = r/20;
  //---------------------------

  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    Cloud* cloud = (Cloud*)collection->selected_obj;

    if(cloud->name.find("Sphere") != std::string::npos){
      vector<vec3>& XYZ = cloud->xyz;
      vector<float>& dist = cloud->R;
      if(dist.size() == 0) {attribManager->compute_Distances(cloud);}

      //Search for nearest point
      float distm, Xm, Ym, Zm;
      float dist_min = fct_min(dist);
      for (int k=0; k<XYZ.size(); k++){
        if(dist[k] == dist_min){
            distm = dist[k];
            Xm = XYZ[k].x;
            Ym = XYZ[k].y;
            Zm = XYZ[k].z;
        }
      }

      //Determine the center of the sphere
      vec3 Center = vec3(Xm + r * (Xm / distm), Ym + r * (Ym / distm), Zm + r * (Zm / distm));

      //For each point supress points too far from radius
      vector<int> idx;
      for(int j=0; j<XYZ.size(); j++){
        float OP = fct_distance(XYZ[j], Center);
        if(OP >= r + err || OP <= r - err){
          idx.push_back(j);
        }
      }

      attribManager->make_supressPoints(cloud, idx);
      sceneManager->update_glyph(collection);
    }
  }

  //---------------------------
}
void Filter::filter_sphere_collection(Collection* collection){
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    this->filter_sphere_cloud(cloud);
  }

  //---------------------------
}
void Filter::filter_sphere_cloud(Cloud* object){
  vector<vec3>& xyz = object->xyz;
  vector<int> idx;
  //---------------------------

  for(int i=0; i<xyz.size(); i++){
    float dist = fct_distance(xyz[i], object->root);
    if(dist < sphere_min || dist > sphere_max){
      idx.push_back(i);
    }
  }

  //Supress points
  int idx_size = idx.size();
  attribManager->make_supressPoints(object, idx);

  //---------------------------
}
void Filter::filter_cylinder_cloud(Collection* collection){
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    this->filter_cylinder_subset(cloud);
  }

  //---------------------------
}
void Filter::filter_cylinder_subset(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<int> idx;
  //---------------------------

  //Check points condition
  for(int i=0; i<XYZ.size(); i++){
    vec3 point = XYZ[i];
    float dist = fct_distance(point, cloud->root);

    if(dist < cyl_r_min || dist > cyl_r_max || point.z < cyl_z_min){
      idx.push_back(i);
    }
  }

  //Supress non valid points
  int idx_size = idx.size();
  attribManager->make_supressPoints(cloud, idx);

  //---------------------------
  if(verbose){
    string result = "Cylinder filtering: " + to_string(idx_size) + " supressed";
    console.AddLog("#", result);
  }
}
