#include "Obstacle.h"

#include "../../Engine/Glyphs.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Object/OOBB.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Operation.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
Obstacle::Obstacle(){
  //---------------------------

  this->glyphManager = new Glyphs();

  this->dir_path = "../media/detection/";
  this->dir_predi = "pred/";
  this->dir_grThr = "gt/";
  this->dir_frame = "frame/";

  //---------------------------
}
Obstacle::~Obstacle(){}

void Obstacle::run(){
  Operation opeManager;
  //---------------------------

  //load frames
  string path_frame = dir_path + dir_frame;
  opeManager.loading_directoryFrames(path_frame);
  Scene sceneManager;
  Cloud* cloud = sceneManager.get_cloud_selected();

  //Load json files - GT
  string path_gt = dir_path + dir_grThr;
  vector<string> paths_gt = opeManager.get_directoryAllFilePath(path_gt);
  this->parse_json(cloud, paths_gt, "gt");
  this->build_obstacleGlyph_gt(cloud);

  //Load json files - predictions
  string path_pr = dir_path + dir_predi;
  vector<string> paths_pr = opeManager.get_directoryAllFilePath(path_pr);
  this->parse_json(cloud, paths_pr, "pr");
  this->build_obstacleGlyph_pr(cloud);

  //---------------------------
}

void Obstacle::parse_json(Cloud* cloud, vector<string> paths, string data){

  if(paths.size() != cloud->subset.size()) return;

  for(int i=0; i<cloud->subset.size(); i++){
    //ieme subset
    Subset* subset = &cloud->subset[i];

    //ieme json frame name
    for(int j=0; j<paths.size(); j++){
      std::ifstream ifs(paths[j]);
      Json::Reader reader;
      Json::Value obj;
      reader.parse(ifs, obj);
      string json_name = obj["frame_id"].asString();

      if(subset->name == json_name){
        //Make stuff
        Obstac* obstacle = &subset->obstacle;
        const Json::Value& json_dete = obj["detections"];

        for (int i = 0; i < json_dete.size(); i++){
          //Obstacle name
          string name = json_dete[i]["name"].asString();

          //Obstacle position
          const Json::Value& json_pos = json_dete[i]["position"];
          vec3 position;
          for (int j=0; j<json_pos.size(); j++){
            position[j] = json_pos[j].asFloat();
          }

          //Obstacle dimension
          const Json::Value& json_dim = json_dete[i]["dimensions"];
          vec3 dimension;
          for (int j=0; j<json_dim.size(); j++){
            dimension[j] = json_dim[j].asFloat();
          }

          //Obstacle heading
          const Json::Value& json_head = json_dete[i]["heading"];
          float heading = json_head.asFloat();

          //Store all data
          if(data == "pr"){
            obstacle->pr_name.push_back(name);
            obstacle->pr_position.push_back(position);
            obstacle->pr_dimension.push_back(dimension);
            obstacle->pr_heading.push_back(heading);
          }
          else if(data == "gt"){
            obstacle->gt_name.push_back(name);
            obstacle->gt_position.push_back(position);
            obstacle->gt_dimension.push_back(dimension);
            obstacle->gt_heading.push_back(heading);
          }
        }
        break;
      }
    }
  }
}
void Obstacle::build_obstacleGlyph_gt(Cloud* cloud){
  OOBB oobbManager;
  Transforms transformManager;
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Obstac* obstacle = &cloud->subset[i].obstacle;

    for(int j=0; j<obstacle->gt_name.size(); j++){
      Glyph* glyph = glyphManager->create_glyph_instance("obstacle");
      glyphManager->set_glyph_color(glyph, vec3(37.0f/255, 186.0f/255, 40.0f/255));

      vec3 To = obstacle->gt_position[j];
      vec3 Ro = vec3(0, 0, obstacle->gt_heading[j]);
      vec3 So = obstacle->gt_dimension[j];
      mat4 transf = transformManager.compute_transformMatrix(To, Ro, So);

      oobbManager.update_oobb(glyph, transf);
      glyphManager->update_glyph_location(glyph);
      obstacle->gt_oobb.push_back(glyph);
    }
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph_pr(Cloud* cloud){
  OOBB oobbManager;
  Transforms transformManager;
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Obstac* obstacle = &cloud->subset[i].obstacle;

    for(int j=0; j<obstacle->pr_name.size(); j++){
      Glyph* glyph = glyphManager->create_glyph_instance("obstacle");
      glyphManager->set_glyph_color(glyph, vec3(201.0f/255, 1.0f/255, 30.0f/255));

      vec3 To = obstacle->pr_position[j];
      vec3 Ro = vec3(0, 0, obstacle->pr_heading[j]);
      vec3 So = obstacle->pr_dimension[j];
      mat4 transf = transformManager.compute_transformMatrix(To, Ro, So);

      oobbManager.update_oobb(glyph, transf);
      glyphManager->update_glyph_location(glyph);
      obstacle->pr_oobb.push_back(glyph);
    }
  }

  //---------------------------
}

void Obstacle::select_dir_path(){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + dir_path;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
    }

    this->dir_path = path_str + "/";
  }

  //---------------------------
}
