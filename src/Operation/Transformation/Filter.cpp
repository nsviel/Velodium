#include "Filter.h"

#include "Attribut.h"

#include "../../Engine/Scene.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_display.h"


//Constructor / Destructor
Filter::Filter(){
  //---------------------------

  this->sceneManager = new Scene();
  this->attribManager = new Attribut();

  this->sphereDiameter = 0.139f;

  //---------------------------
}
Filter::~Filter(){}

//Functions
void Filter::filter_maxAngle(Cloud* cloud, float angleMax){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  attribManager->compute_cloudAttributs(subset);
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
  int size_filtered = subset->xyz.size();
  console.AddLog("Filter by angle (%.2f°) %s : %i -> %i points (%.2f ms)", angleMax, cloud->subset[0].name.c_str(), size_before, size_filtered, duration);
}
void Filter::filter_sphereCleaning(){
  list<Cloud*>* list_Cloud = database.list_cloud;
  float r = sphereDiameter/2;
  float err = r/20;
  //---------------------------

  for(int i=0; i<list_Cloud->size(); i++){
    Cloud* cloud = *next(list_Cloud->begin(),i);
    Subset* subset = &cloud->subset[cloud->subset_selected];

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
