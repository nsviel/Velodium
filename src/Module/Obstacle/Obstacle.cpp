#include "Obstacle.h"

#include "Interfacing.h"

#include "../Module_node.h"
#include "../LiDAR/Capture.h"

#include "../../Engine/Scene.h"
#include "../../Engine/Glyphs.h"
#include "../../Engine/Object/OOBB.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Obstacle::Obstacle(Module_node* node_module){
  //---------------------------

  this->ioManager = node_module->get_ioManager();
  this->captureManager = node_module->get_captureManager();
  this->sceneManager = new Scene();
  this->oobbManager = new OOBB();
  this->transformManager = new Transforms();
  this->glyphManager = new Glyphs();

  bool save_frame = true;
  bool save_image = true;

  //---------------------------
}
Obstacle::~Obstacle(){}

void Obstacle::runtime_obstacle(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  //---------------------------

  if(captureManager->get_is_capturing() && cloud != nullptr){
    bool new_obstacle = ioManager->check_prediction(cloud);
    if(new_obstacle){
      this->build_obstacleGlyph_gt(cloud);
    }

    //Save subset frame
    if(save_frame){
      ioManager->save_frame(cloud);
    }

    //Save rendered image
    if(save_image){
      ioManager->save_image();
    }
  }

  //---------------------------
}
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
