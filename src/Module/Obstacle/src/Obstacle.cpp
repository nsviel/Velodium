#include "Obstacle.h"
#include "Warning.h"
#include "Prediction.h"

#include "../Module_obstacle.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Object.h"
#include "../../../Engine/Scene/Glyph/Cloud/OOBB.h"
#include "../../../Operation/Transformation/Pose.h"
#include "../../../Specific/color.h"
#include "../../../Interface/File/Directory.h"
#include "../../../Interface/File/Zenity.h"


//Constructor / Destructor
Obstacle::Obstacle(Module_obstacle* module){
  //---------------------------

  Node_engine* node_engine = module->get_node_engine();

  this->predManager = module->get_predManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();
  this->warningManager = module->get_warningManager();
  this->oobbManager = new OOBB();
  this->poseManager = new Pose();

  this->with_warning = false;
  this->with_prediction = true;
//configManager->parse_json_b("module", "with_prediction")
  //---------------------------
}
Obstacle::~Obstacle(){}

//Main functions
void Obstacle::runtime(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  if(with_prediction && cloud != nullptr){
    //Check for new prediction (ground thruth or prediction)
    bool* is_new_pred = predManager->runtime_prediction();

    if(*is_new_pred){
      this->build_cloud_obstacle(cloud);

      //Reverse flag
      *is_new_pred = false;
    }
  }

  //---------------------------
}

//Manual obstacle adding
void Obstacle::add_detectioned(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = zenity_file_vec("Prediction loading");

  //Parses predictions files
  predManager->compute_prediction(cloud, path_vec);

  //Build glyphs
  this->build_cloud_obstacle(cloud);

  //---------------------------
}
void Obstacle::add_detectioned(string path_dir){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = list_all_path(path_dir);

  //Parses predictions files
  predManager->compute_prediction(cloud, path_vec);

  //Build glyphs
  this->build_cloud_obstacle(cloud);

  //---------------------------
}
void Obstacle::add_obstacle_grTr(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Get prediction file paths
  vector<string> path_vec = zenity_file_vec("Ground truth loading");

  //Parses predictions files
  predManager->compute_groundTruth(cloud, path_vec);

  //Build glyphs
  this->build_cloud_obstacle(cloud);

  //---------------------------
}

//Subfunctions
void Obstacle::build_cloud_obstacle(Cloud* cloud){
  //---------------------------

  //Process prediction if the ieme subset are not already processed
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->detection.is_predicted == false){
      //Build obstacle glyph
      this->build_obstacleGlyph_pr(subset);
      this->build_obstacleGlyph_gt(subset);

      //Send MQTT warning
      if(with_warning){
        warningManager->send_warning(subset);
      }
    }
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_pr(Subset* subset){
  Data_pred* detection = &subset->detection;
  //---------------------------

  for(int i=0; i<detection->name.size(); i++){
    Glyph* glyph = objectManager->create_glyph_ostacle();

    vec3 To = detection->position[i];
    vec3 Ro = vec3(0, 0, detection->heading[i]);
    vec3 So = detection->dimension[i];
    mat4 transf = poseManager->compute_transformMatrix(To, Ro, So);
    vec4 color = AI_color_dic.find(detection->name[i])->second;

    oobbManager->update_oobb(glyph, transf);
    objectManager->update_object(glyph, color);
    detection->oobb.push_back(*glyph);

    detection->is_predicted == true;
    delete glyph;
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_gt(Subset* subset){
  Data_pred* detection_gt = &subset->detection_gt;
  //---------------------------

  for(int j=0; j<detection_gt->name.size(); j++){
    Glyph* glyph = objectManager->create_glyph_ostacle();

    vec3 To = detection_gt->position[j];
    vec3 Ro = vec3(0, 0, detection_gt->heading[j]);
    vec3 So = detection_gt->dimension[j];
    mat4 transf = poseManager->compute_transformMatrix(To, Ro, So);

    oobbManager->update_oobb(glyph, transf);
    objectManager->update_object(glyph);
    detection_gt->oobb.push_back(*glyph);
    delete glyph;
  }

  //---------------------------
}
