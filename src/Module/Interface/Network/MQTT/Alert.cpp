#include "Alert.h"

#include "MQTT.h"


//Constructor / Destructor
Alert::Alert(MQTT* mqtt){
  this->mqttManager = mqtt;
  //---------------------------


  //---------------------------
}
Alert::~Alert(){}

void Alert::send_prediction_by_mqtt(Subset* subset){
  //---------------------------

  Obstac* obstacle = &subset->obstacle_pr;
  string alert_json = "{\"detections\": [";

  for(int i=0; i<obstacle->name.size(); i++){
    //Obstacle begin
    alert_json += "{";

    //Most similar label
    alert_json += "\"label\": \"" + obstacle->name[i] + "\",";

    //dimension
    alert_json += "\"dimension\": [";
    alert_json += to_string(obstacle->dimension[i].x) + ",";
    alert_json += to_string(obstacle->dimension[i].y) + ",";
    alert_json += to_string(obstacle->dimension[i].z) + "]";

    //Obstacle end
    alert_json += "},";
  }

  alert_json += "]}";

  say(alert_json);

  mqttManager->mqtt_send_message(alert_json);

  //---------------------------
}
