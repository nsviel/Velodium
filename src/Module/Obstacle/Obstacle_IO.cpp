#include "Obstacle_IO.h"

#include "../../Specific/fct_system.h"
#include "../../Engine/Scene.h"
#include "../../Load/Operation.h"
#include "../../Load/Saver.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


#include "notify.h"


//Constructor / Destructor
Obstacle_IO::Obstacle_IO(){
  //---------------------------

  this->saverManager = new Saver();

  this->dir_path = get_absolutePath_build() + "/../media/data/capture/";
  this->dir_frame = dir_path + "frame/";
  this->dir_predi = dir_path + "prediction/";
  this->dir_grThr = dir_path + "groundtruth/";
  this->savedFrame_ID = 0;
  this->savedFrame_max = 20;

  this->clean_directories();

  //---------------------------
}
Obstacle_IO::~Obstacle_IO(){}

void Obstacle_IO::load_obstacleData(){
  Operation opeManager;
  //---------------------------

  sayHello();
  m_thread = std::thread([&]() {
    while (true) {
      dir_modif_watcher(dir_path);
    }
  });
  sayHello();

  //load frames
  string path_frame = dir_path + dir_frame;
  opeManager.loading_directoryFrames(path_frame);
  Scene sceneManager;
  Cloud* cloud = sceneManager.get_cloud_selected();

  //Load json files - GT
  string path_gt = dir_path + dir_grThr;
  vector<string> paths_gt = opeManager.get_directoryAllFilePath(path_gt);
  this->parse_obstacle_json(cloud, paths_gt, "gt");

  //Load json files - predictions
  string path_pr = dir_path + dir_predi;
  vector<string> paths_pr = opeManager.get_directoryAllFilePath(path_pr);
  this->parse_obstacle_json(cloud, paths_pr, "pr");

  //---------------------------
}
void Obstacle_IO::clean_directories(){
  //---------------------------

  clean_directory_files(dir_frame.c_str());
  clean_directory_files(dir_predi.c_str());
  clean_directory_files(dir_grThr.c_str());

  //---------------------------
}
void Obstacle_IO::save_nFrame(Cloud* cloud){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  //---------------------------

  string filePath = dir_frame + subset->name + ".ply";
  saverManager->save_subset(subset, "ply", dir_frame);

  if(save_path_vec.size() < savedFrame_max){
    save_path_vec.push_back(filePath);
    savedFrame_ID++;
  }else{
    std::remove (save_path_vec[savedFrame_ID].c_str());
    save_path_vec[savedFrame_ID] = filePath;
    savedFrame_ID++;
  }

  if(savedFrame_ID >= savedFrame_max){
    savedFrame_ID = 0;
  }

  //---------------------------
}
void Obstacle_IO::parse_obstacle_json(Cloud* cloud, vector<string> paths, string data){
  //---------------------------

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
        Obstac* obstacle_gt = &subset->obstacle_gt;
        Obstac* obstacle_pr = &subset->obstacle_pr;
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
            obstacle_pr->name.push_back(name);
            obstacle_pr->position.push_back(position);
            obstacle_pr->dimension.push_back(dimension);
            obstacle_pr->heading.push_back(heading);
          }
          else if(data == "gt"){
            obstacle_gt->name.push_back(name);
            obstacle_gt->position.push_back(position);
            obstacle_gt->dimension.push_back(dimension);
            obstacle_gt->heading.push_back(heading);
          }
        }
        break;
      }
    }
  }

  //---------------------------
}
void Obstacle_IO::select_dir_path(){
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
