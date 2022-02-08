#include "Network.h"

#include "Protocol/MQTT/MQTT.h"
#include "Protocol/SFTP.h"
#include "Protocol/SSH.h"

#include "../../Specific/fct_maths.h"
#include "../../Load/Pather.h"


//Constructor / Destructor
Network::Network(){
  //---------------------------

  this->mqttManager = new MQTT();
  this->sftpManager = new SFTP();
  this->sshManager = new SSH();

  this->path_source = "/home/aither/Desktop/Point_cloud/frame_0001.ply";
  this->path_target = "/home/aither/Desktop/";
  this->name_file = "frame_0001.ply";

  this->is_connected = false;

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

//Subfunctions
void Network::select_sourcePath(){
  //---------------------------

  //Select file to send
  Pather pathManager;
  path_source = pathManager.get_filePath(path_source);

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
  Pather pathManager;
  pathManager.selectDirectory(path_target);

  //Supress unwanted line break
  if (path_source.find('\n')){
    path_target.erase(std::remove(path_target.begin(), path_target.end(), '\n'), path_target.end());
  }

  //---------------------------
}
