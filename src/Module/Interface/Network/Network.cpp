#include "Network.h"

#include "MQTT/MQTT.h"
#include "MQTT/Alert.h"
#include "SFTP/SFTP.h"
#include "SFTP/SSH.h"

#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Network::Network(){
  //---------------------------

  this->mqttManager = new MQTT();
  this->alertManager = new Alert(mqttManager);
  this->sftpManager = new SFTP();
  this->sshManager = new SSH();

  this->path_source = "/home/aether/Desktop/Point_cloud/frame_0001.ply";
  this->path_target = "/home/aether/Desktop/";
  this->name_file = "frame_0001.ply";

  this->is_connected = false;
  this->is_image_watcher = false;
  this->is_mqtt_watcher = false;

  //---------------------------
}
Network::~Network(){}

//Main function
void Network::start_connection(){
  //---------------------------

  sshManager->ssh_startConnexion();
  this->is_connected = *sshManager->get_ssh_connected();

  //---------------------------
}
void Network::stop_connection(){
  //---------------------------

  sshManager->ssh_stopConnexion();
  this->is_connected = *sshManager->get_ssh_connected();

  //---------------------------
}
void Network::send_file(string path_source, string path_target){
  //---------------------------

  //Check if ssh is established
  bool ssh_connected = *sshManager->get_ssh_connected();
  if(ssh_connected == false){
    cout<<"SSH not connected."<<endl;
    return;
  }

  //Send file
  ssh_session* ssh = sshManager->get_ssh_session();
  sftpManager->sftp_sendFile(*ssh, path_source, path_target);

  //---------------------------
}

//Threaded functions
void Network::start_thread_mqtt(){
  //---------------------------

  //mqttManager->mqtt_sendMessages();

  //---------------------------
}
void Network::start_thread_image(){
  //---------------------------

  //mqttManager->mqtt_sendMessages();

  //---------------------------
}

//Subfunctions
void Network::select_sourcePath(){
  //---------------------------

  //Select file to send
  zenity_file("source", path_source);

  //Supress unwanted line break
  if (path_source.find('\n')){
    path_source.erase(std::remove(path_source.begin(), path_source.end(), '\n'), path_source.end());
  }

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

  //Supress unwanted line break
  if (path_source.find('\n')){
    path_target.erase(std::remove(path_target.begin(), path_target.end(), '\n'), path_target.end());
  }

  //---------------------------
}
