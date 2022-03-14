#include "Warning.h"

#include "../../Interface/Network/MQTT/MQTT.h"


//Constructor / Destructor
Warning::Warning(MQTT* mqtt){
  this->mqttManager = mqtt;
  //---------------------------


  //---------------------------
}
Warning::~Warning(){}

void Warning::send_warning(Subset* subset){
  //---------------------------

  string warning = build_jsonFile(subset);
  mqttManager->mqtt_send_message(warning);

  //---------------------------
}
string Warning::build_jsonFile(Subset* subset){
  Obstac* obstacle = &subset->obstacle_pr;
  //---------------------------

  //Begin
  string warning = "{\"detections\": [";

  //Middle
  for(int i=0; i<obstacle->name.size(); i++){
    //Obstacle begin
    warning += "{";

    //Most similar label
    warning += "\"label\": \"" + obstacle->name[i] + "\",";

    //dimension
    warning += "\"dimension\": [";
    warning += to_string(obstacle->dimension[i].x) + ",";
    warning += to_string(obstacle->dimension[i].y) + ",";
    warning += to_string(obstacle->dimension[i].z) + "]";

    //Obstacle end
    warning += "},";
  }

  //End
  warning += "]}";

  //---------------------------
  return warning;
}
