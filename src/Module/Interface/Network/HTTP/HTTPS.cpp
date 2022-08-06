#include "HTTPS.h"

#include "Daemon.h"

#include "../Network.h"

#include "../../Interface_node.h"
#include "../../Local/Saving.h"

#include "../../../../Engine/Engine_node.h"
#include "../../../../Engine/Scene/Configuration.h"


//Constructor / Destructor
HTTPS::HTTPS(Network* netManager){
  //---------------------------

  Interface_node* node_interface = netManager->get_node_interface();
  Engine_node* node_engine = node_interface->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->saveManager = node_interface->get_saveManager();
  this->daemonManager = new Daemon();

  //---------------------------
  this->update_configuration();
  this->start_server();
}
HTTPS::~HTTPS(){}

void HTTPS::update_configuration(){
  //---------------------------

  this->path_image = saveManager->get_path_image() + "image";
  this->with_http_demon = configManager->parse_json_b("network", "with_http_demon");
  this->server_port = configManager->parse_json_i("network", "http_port");

  //---------------------------
}
void HTTPS::start_server(){
  //---------------------------

  if(with_http_demon){
    daemonManager->set_configuration(server_port);
    daemonManager->set_path_image(path_image);
    daemonManager->start_deamon();

    this->is_https_deamon = daemonManager->get_is_daemon();
  }

  //---------------------------
}
void HTTPS::stop_server(){
  //---------------------------

  if(with_http_demon){
    daemonManager->stop_deamon();
  }

  //---------------------------
}
