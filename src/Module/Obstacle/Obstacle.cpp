#include "Obstacle.h"

#include "../../Engine/Glyphs.h"
#include "../../Engine/Object/OOBB.h"
#include "../../Operation/Transformation/Transforms.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>


//Constructor / Destructor
Obstacle::Obstacle(){
  //---------------------------

  this->glyphManager = new Glyphs();

  //---------------------------
}
Obstacle::~Obstacle(){}

void Obstacle::run(){
  //---------------------------

  string path = "/home/aither/Desktop/pred_thresh_0.4_1616100800400.json";
  this->parse_json(path);
  this->build_obstacleGlyph();

  //---------------------------
}
void Obstacle::parse_json(string path){
  detect_name.clear();
  detect_posi.clear();
  detect_dime.clear();
  detect_head.clear();
  //---------------------------

  std::ifstream ifs(path);

  Json::Reader reader;
  Json::Value obj;

  reader.parse(ifs, obj);

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
    detect_name.push_back(name);
    detect_posi.push_back(position);
    detect_dime.push_back(dimension);
    detect_head.push_back(heading);
  }

  //---------------------------
}
void Obstacle::build_obstacleGlyph(){
  OOBB oobbManager;
  Transforms transformManager;
  //---------------------------

  for(int i=0; i<detect_name.size(); i++){
    Glyph* glyph = glyphManager->create_glyph_instance("obstacle");

    vec3 To = detect_posi[i];
    vec3 Ro = vec3(0, 0, detect_head[i]);
    vec3 So = detect_dime[i];
    mat4 transf = transformManager.compute_transformMatrix(To, Ro, So);

    oobbManager.update_oobb(glyph, transf);
    glyphManager->update_glyph_location(glyph);
  }

  //---------------------------
}
