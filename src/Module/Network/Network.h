#ifndef NETWORK_H
#define NETWORK_H

#include "../../common.h"

#include <libssh/libssh.h>

class MQTT;
class SFTP;
class SSH;


class Network
{
public:
  //Constructor / Destructor
  Network();
  ~Network();

public:
  void send_file();

  void select_sourcePath();
  void select_targetPath();

  inline MQTT* get_mqttManager(){return mqttManager;}
  inline SFTP* get_sftpManager(){return sftpManager;}
  inline SSH* get_sshManager(){return sshManager;}
  inline string* get_path_source(){return &path_source;}
  inline string* get_path_target(){return &path_target;}

private:
  MQTT* mqttManager;
  SFTP* sftpManager;
  SSH* sshManager;

  string name_file;
  string path_source;
  string path_target;
};

#endif
