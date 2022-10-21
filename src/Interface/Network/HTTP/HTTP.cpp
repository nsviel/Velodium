#include "HTTP.h"
#include "http_daemon.h"

#include "../../Node_interface.h"

#include "../../../Engine/Scene/Configuration.h"


//Constructor / Destructor
HTTP::HTTP(Node_interface* node_interface){
  //---------------------------

  this->configManager = node_interface->get_configManager();
  this->daemonManager = new http_daemon();

  //---------------------------
  this->update_configuration();
  this->start_server();
}
HTTP::~HTTP(){}

void HTTP::update_configuration(){
  //---------------------------

  this->path_image = configManager->parse_json_s("parameter", "path_data") + "image";
  this->with_http_demon = configManager->parse_json_b("network", "with_http_demon");
  this->server_port = configManager->parse_json_i("network", "http_port");

  //---------------------------
}
void HTTP::start_server(){
  //---------------------------

  if(with_http_demon){
    daemonManager->set_configuration(server_port);
    daemonManager->set_path_image(path_image);
    daemonManager->start_deamon();

    this->is_https_deamon = daemonManager->get_is_daemon();
  }

  //---------------------------
}
void HTTP::stop_server(){
  //---------------------------

  if(with_http_demon){
    daemonManager->stop_deamon();
  }

  //---------------------------
}
