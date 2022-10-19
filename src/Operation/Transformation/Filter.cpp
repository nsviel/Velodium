#include "Filter.h"

#include "Attribut.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_terminal.h"


//Constructor / Destructor
Filter::Filter(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();

  //---------------------------
  this->update_configuration();
}
Filter::~Filter(){}

void Filter::update_configuration(){
  //---------------------------

  this->verbose = false;
  this->sphereDiameter = 0.139f;
  this->cyl_r_min = configManager->parse_json_f("parameter", "filter_cylinder_rmin");
  this->cyl_r_max = configManager->parse_json_f("parameter", "filter_cylinder_rmax");
  this->cyl_z_min = -3;

  //---------------------------
}

//Functions
void Filter::filter_maxAngle(Cloud* cloud, float angleMax){
  Subset* subset = cloud->subset_selected;
  attribManager->compute_attribut_subset(subset);
  vector<float>& It = subset->It;
  int size_before = subset->nb_point;
  tic();
  //---------------------------

  vector<int> idx;
  for(int i=0; i<It.size(); i++){
    if(It[i] >= angleMax){
      idx.push_back(i);
    }
  }

  //Supress points with angle superior to the limit
  attribManager->make_supressPoints(subset, idx);

  //---------------------------
  float duration = toc();
  if(verbose){
    int size_filtered = subset->xyz.size();
    string log = "Filter by angle (" + to_string(angleMax) + "°) : " + to_string(size_before) + " -> " + to_string(size_filtered) + " points (" + to_string(duration) + " ms)";
    console.AddLog("ok", log);
  }
}
void Filter::filter_sphereCleaning(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  float r = sphereDiameter/2;
  float err = r/20;
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    if(subset->name.find("Sphere") != std::string::npos){
      vector<vec3>& XYZ = subset->xyz;
      vector<float>& dist = subset->R;
      if(dist.size() == 0) {attribManager->compute_Distances(subset);}

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

      attribManager->make_supressPoints(subset, idx);
      sceneManager->update_cloud_glyphs(cloud);
    }
  }

  //---------------------------
}
void Filter::filter_subset_cylinder(Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vector<int> idx;
  //---------------------------

  //Check points condition
  for(int i=0; i<XYZ.size(); i++){
    vec3 point = XYZ[i];
    float dist = fct_distance(point, subset->root);

    if(dist < cyl_r_min || dist > cyl_r_max || point.z < cyl_z_min){
      idx.push_back(i);
    }

  }

  //Supress non valid points
  int idx_size = idx.size();
  attribManager->make_supressPoints(subset, idx);

  //---------------------------
  if(verbose){
    string result = "Cylinder filtering: " + to_string(idx_size) + " supressed";
    console.AddLog("#", result);
  }
}
void Filter::filter_cloud_cylinder(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->filter_subset_cylinder(subset);
  }

  //---------------------------
}
