#include "Obstacle.h"

#include "Scenario.h"

#include "../../Module_node.h"
#include "../../Interface/Interface.h"
#include "../../Interface/Component/Prediction.h"
#include "../../Interface/LiDAR/Capture.h"

#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyphs.h"
#include "../../../Engine/Scene/Object/OOBB.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Load/Pather.h"


//Constructor / Destructor
Obstacle::Obstacle(Module_node* node_module){
  //---------------------------

  this->ioManager = node_module->get_ioManager();
  this->captureManager = ioManager->get_captureManager();
  this->predManager = ioManager->get_predManager();
  this->sceneManager = new Scene();
  this->oobbManager = new OOBB();
  this->transformManager = new Transforms();
  this->glyphManager = new Glyphs();

  //---------------------------
}
Obstacle::~Obstacle(){}

//Main functions
void Obstacle::runtime_obstacle(){
  //This function is called at each OpenGL iteration
  Cloud* cloud = sceneManager->get_cloud_selected();
  Prediction* predManager = ioManager->get_predManager();
  //---------------------------

  if(predManager->get_is_watching() && cloud != nullptr){

    //Check for new prediction (ground thruth or prediction)
    bool is_prediction = predManager->get_is_prediction();
    if(is_prediction){
      say("new prediction ok");
      this->build_obstacleGlyph_gt(cloud);
      this->build_obstacleGlyph_pr(cloud);
    }
  }

  //---------------------------
}
void Obstacle::compute_obstacle(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = pathManager->zenity_loading("Prediction loading");

  //Parses predictions files
  predManager->compute_prediction(cloud, path_vec);

  //Build glyphs
  this->build_obstacleGlyph_pr(cloud);

  //---------------------------
}
void Obstacle::compute_groundTruth(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = pathManager->zenity_loading("Ground truth loading");

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
    Obstac* obstacle_gt = &subset->obstacle_gt;

    for(int j=0; j<obstacle_gt->name.size(); j++){
      Glyph* glyph = glyphManager->create_glyph_instance("obstacle");
      glyphManager->set_glyph_color(glyph, vec3(37.0f/255, 186.0f/255, 40.0f/255));

      vec3 To = obstacle_gt->position[j];
      vec3 Ro = vec3(0, 0, obstacle_gt->heading[j]);
      vec3 So = obstacle_gt->dimension[j];
      mat4 transf = transformManager->compute_transformMatrix(To, Ro, So);

      oobbManager->update_oobb(glyph, transf);
      glyphManager->update_glyph_location(glyph);
      obstacle_gt->oobb.push_back(glyph);
    }
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_pr(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    Obstac* obstacle_pr = &subset->obstacle_pr;

    for(int j=0; j<obstacle_pr->name.size(); j++){
      Glyph* glyph = glyphManager->create_glyph_instance("obstacle");
      glyphManager->set_glyph_color(glyph, vec3(201.0f/255, 1.0f/255, 30.0f/255));

      vec3 To = obstacle_pr->position[j];
      vec3 Ro = vec3(0, 0, obstacle_pr->heading[j]);
      vec3 So = obstacle_pr->dimension[j];
      mat4 transf = transformManager->compute_transformMatrix(To, Ro, So);

      oobbManager->update_oobb(glyph, transf);
      glyphManager->update_glyph_location(glyph);
      obstacle_pr->oobb.push_back(glyph);
    }
  }

  //---------------------------
}
