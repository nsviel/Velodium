#include "file_JSON.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


//Constructor / Destructor
file_JSON::file_JSON(){
  //---------------------------


  //---------------------------
}
file_JSON::~file_JSON(){}

void file_JSON::parse_json_obstacle(Collection* collection, vector<string> paths, string data){
  //---------------------------
/*
  if(paths.size() != collection->list_obj.size()) return;

  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);

    //ieme json frame name
    for(int j=0; j<paths.size(); j++){
      std::ifstream ifs(paths[j]);
      Json::Reader reader;
      Json::Value obj;
      reader.parse(ifs, obj);
      string json_name = obj["frame_id"].asString();

      if(cloud->name == json_name){
        //Make stuff
        Data_pred* detection_gt = &cloud->detection_gt;
        Data_pred* detection = &cloud->detection;
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
            detection->name.push_back(name);
            detection->position.push_back(position);
            detection->dimension.push_back(dimension);
            detection->heading.push_back(heading);
          }
          else if(data == "gt"){
            detection_gt->name.push_back(name);
            detection_gt->position.push_back(position);
            detection_gt->dimension.push_back(dimension);
            detection_gt->heading.push_back(heading);
          }
        }
        break;
      }
    }
  }
*/
  //---------------------------
}
