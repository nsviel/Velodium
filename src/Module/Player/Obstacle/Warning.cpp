#include "Warning.h"

#include "../../Interface/Network/MQTT/MQTT.h"
#include "../../Interface/Network/MQTT/PAHO.h"


//Constructor / Destructor
Warning::Warning(MQTT* mqtt){
  this->mqttManager = mqtt;
  //---------------------------


  //---------------------------
}
Warning::~Warning(){}

void Warning::send_warning(Subset* subset){
  PAHO* mqtt_sncf = mqttManager->get_mqtt_sncf();
  //---------------------------

  string warning = build_jsonFile(subset);
  mqtt_sncf->paho_connection();
  mqtt_sncf->paho_publish(warning);

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
