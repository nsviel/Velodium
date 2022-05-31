#include "Network.h"

#include "MQTT/MQTT.h"
#include "SFTP/SFTP.h"
#include "SFTP/SSH.h"
#include "HTTP/HTTPS.h"

#include "../Interface_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Network::Network(Interface_node* node){
  this->node_interface = node;
  //---------------------------

  Engine_node* node_engine = node->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->create_wallet();
  
  this->mqttManager = new MQTT(this);
  this->sshManager = new SSH();
  this->sftpManager = new SFTP(this);
  this->httpsManager = new HTTPS(this);

  this->is_connected = false;
  this->is_image_watcher = false;
  this->is_mqtt_watcher = false;

  this->path_source = "/home/aether/Desktop/Point_cloud/frame_0001.ply";
  this->path_target = "/home/aether/Desktop/";
  this->name_file = "frame_0001.ply";

  //---------------------------
}
Network::~Network(){}

//Main function
void Network::update_configuration(){
  //---------------------------

  httpsManager->update_configuration();
  mqttManager->update_configuration();

  //---------------------------
}
void Network::create_wallet(){
  //---------------------------

  this->wallet = new Wallet();
  this->wallet->wallet_dic = configManager->parse_json_dict("wallet");
  this->wallet->make_concatenation();

  //---------------------------
}
string Network::get_ip_from_dest(string dest){
  //---------------------------

  string ip = wallet->wallet_dic[dest];

  //---------------------------
  return ip;
}

//Connection function
void Network::start_connection(){
  //---------------------------

  sshManager->ssh_connection();
  this->is_connected = *sshManager->get_ssh_connected();

  //---------------------------
}
void Network::stop_connection(){
  //---------------------------

  sshManager->ssh_disconnection();
  this->is_connected = *sshManager->get_ssh_connected();

  //---------------------------
}
void Network::select_sourcePath(){
  //---------------------------

  //Select file to send
  zenity_file(path_source);

  //Retrieve name file
  name_file = path_source.substr(path_source.find_last_of("/\\") + 1);

  //Supress unwanted line break
  if (name_file.find('\n')){
    name_file.erase(std::remove(name_file.begin(), name_file.end(), '\n'), name_file.end());
  }

  //---------------------------
}
void Network::select_targetPath(){
  //---------------------------

  //Select file to send
  zenity_directory(path_target);

  //---------------------------
}
void Network::send_file(){
  //---------------------------

  //SSH connection
  sshManager->ssh_connection();

  //Send file
  sftpManager->sftp_new_session();
  sftpManager->sftp_send_file(path_source, path_target, name_file);

  //---------------------------
}
