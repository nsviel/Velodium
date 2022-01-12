#include "Network.h"

#include "Protocol/MQTT.h"
#include "Protocol/SFTP.h"
#include "Protocol/SSH.h"

#include "../../Specific/fct_maths.h"
#include "../../Operation/Operation.h"


//Constructor / Destructor
Network::Network(){
  //---------------------------

  this->mqttManager = new MQTT();
  this->sftpManager = new SFTP();
  this->sshManager = new SSH();

  this->path_source = "/home/aither/Desktop/frame_0001.ply";
  this->path_target = "/home/*/Desktop/";
  this->name_file = "frame_0001.ply";

  //---------------------------
}
Network::~Network(){}

void Network::send_file(){
  //---------------------------

  //Check if ssh is established
  bool* ssh_connected = sshManager->get_ssh_connected();
  if(*ssh_connected == false){
    cout<<"SSH not connected."<<endl;
    return;
  }

  ssh_session* ssh = sshManager->get_ssh_session();
  string path_trg = path_target + name_file;
  sftpManager->sftp_sendFile(*ssh, path_source, path_trg);

  //---------------------------
}

void Network::select_sourcePath(){
  //---------------------------

  //Select file to send
  Operation opeManager;
  path_source = opeManager.get_filePath(path_source);

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
  Operation opeManager;
  opeManager.selectDirectory(path_target);

  //Supress unwanted line break
  if (path_source.find('\n')){
    path_target.erase(std::remove(path_target.begin(), path_target.end(), '\n'), path_target.end());
  }

  //---------------------------
}
