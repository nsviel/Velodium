#include "Obstacle.h"

#include "Scenario.h"

#include "../../Module_node.h"
#include "../../Interface/Interface_node.h"
#include "../../Interface/Local/Prediction.h"
#include "../../Interface/Network/Network.h"
#include "../../Interface/Network/MQTT/Alert.h"
#include "../../Interface/LiDAR/Capture.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyphs.h"
#include "../../../Engine/Scene/Object/OOBB.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Obstacle::Obstacle(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();
  Interface_node* node_interface = node_module->get_node_interface();
  Network* netManager = node_interface->get_netManager();

  this->alertManager = netManager->get_alertManager();
  this->captureManager = node_interface->get_captureManager();
  this->predManager = node_interface->get_predManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->oobbManager = new OOBB();
  this->transformManager = new Transforms();

  this->gt_color = vec4(0, 1, 0, 1.0f);
  this->pr_color = vec4(0.1, 0.1, 0.1, 1.0f);

  //---------------------------
}
Obstacle::~Obstacle(){}

//Main functions
void Obstacle::runtime_obstacle(){
  Subset* subset = sceneManager->get_subset_selected();
  //---------------------------

  if(predManager->get_is_watching() && subset != nullptr){
    //Check for new prediction (ground thruth or prediction)
    bool* is_prediction = predManager->get_is_prediction();

    if(*is_prediction){
      //Build obstacle glyphs
      this->build_obstacleGlyph_gt(subset);
      this->build_obstacleGlyph_pr(subset);

      //Send obstacle warning
      //alertManager->send_prediction_by_mqtt(subset);

      //Reverse flag
      *is_prediction = false;
    }
  }

  //---------------------------
}

//Manual obstacle adding
void Obstacle::add_prediction(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = zenity_file_vec("Prediction loading", "");

  //Parses predictions files
  predManager->compute_prediction(cloud, path_vec);

  //Build glyphs
  this->build_obstacleGlyph_pr(cloud);

  //---------------------------
}
void Obstacle::add_prediction(string path_dir){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = list_allPaths(path_dir);

  //Parses predictions files
  predManager->compute_prediction(cloud, path_vec);

  //Build glyphs
  this->build_obstacleGlyph_pr(cloud);

  //---------------------------
}
void Obstacle::add_groundTruth(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = zenity_file_vec("Ground truth loading", "");

  //Parses predictions files
  predManager->compute_groundTruth(cloud, path_vec);

  //Build glyphs
  this->build_obstacleGlyph_gt(cloud);

  //---------------------------
}

//Subfunctions
void Obstacle::build_obstacleGlyph_gt(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->build_obstacleGlyph_gt(subset);
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_gt(Subset* subset){
  Obstac* obstacle_gt = &subset->obstacle_gt;
  //---------------------------

  for(int j=0; j<obstacle_gt->name.size(); j++){
    Glyph* glyph = glyphManager->create_glyph_ostacle();

    vec3 To = obstacle_gt->position[j];
    vec3 Ro = vec3(0, 0, obstacle_gt->heading[j]);
    vec3 So = obstacle_gt->dimension[j];
    mat4 transf = transformManager->compute_transformMatrix(To, Ro, So);

    oobbManager->update_oobb(glyph, transf);
    glyphManager->update_glyph_location(glyph);
    glyphManager->update_glyph_color(glyph, gt_color);
    obstacle_gt->oobb.push_back(*glyph);
    delete glyph;
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_pr(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->build_obstacleGlyph_pr(subset);
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_pr(Subset* subset){
  Obstac* obstacle_pr = &subset->obstacle_pr;
  //---------------------------

  for(int j=0; j<obstacle_pr->name.size(); j++){
    Glyph* glyph = glyphManager->create_glyph_ostacle();

    vec3 To = obstacle_pr->position[j];
    vec3 Ro = vec3(0, 0, obstacle_pr->heading[j]);
    vec3 So = obstacle_pr->dimension[j];
    mat4 transf = transformManager->compute_transformMatrix(To, Ro, So);

    oobbManager->update_oobb(glyph, transf);
    glyphManager->update_glyph_location(glyph);
    glyphManager->update_glyph_color(glyph, pr_color);
    obstacle_pr->oobb.push_back(*glyph);


    //delete glyph;
  }

  //---------------------------
}
