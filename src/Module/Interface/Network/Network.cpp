#include "Network.h"

#include "MQTT/MQTT.h"
#include "SFTP/SFTP.h"
#include "SFTP/SSH.h"

#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Network::Network(){
  //---------------------------

  this->mqttManager = new MQTT();
  this->sshManager = new SSH();
  this->sftpManager = new SFTP(sshManager);

  this->path_source = "/home/aether/Desktop/Point_cloud/frame_0001.ply";
  this->path_target = "/home/aether/Desktop/";
  this->name_file = "frame_0001.ply";

  this->is_connected = false;
  this->is_image_watcher = false;
  this->is_mqtt_watcher = false;

  //---------------------------
  this->create_wallet();
}
Network::~Network(){}

//Main function
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
void Network::send_file(){
  //---------------------------

  //SSH connection
  sshManager->ssh_connection();

  //Send file
  sftpManager->sftp_new_session();
  sftpManager->sftp_send_file(path_source, path_target, name_file);

  //---------------------------
}

//Subfunctions
void Network::create_wallet(){
  //---------------------------

  this->wallet = new Wallet();
  this->wallet->wallet_dic = {
    { "localhost", "127.0.0.1" },
    { "mine_ordi_nathan", "10.201.20.106" },
    { "mine_ordi_louis", "10.201.20.110" },
    { "mine_server", "10.201.224.13" },
    { "home_portable_nathan", "192.168.1.27" },
  };
  this->wallet->make_concatenation();

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
