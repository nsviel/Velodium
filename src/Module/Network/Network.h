#ifndef NETWORK_H
#define NETWORK_H

#include "../../common.h"

#include <libssh/sftp.h>
#include <libssh/libssh.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <filesystem>
#include <fcntl.h>


class Network
{
public:
  //Constructor / Destructor
  Network();
  ~Network();

public:
  void ssh_connexion();
  void sftp_sendFile();
  void sftp_createDirectory(string dirName);

  void ssh_startConnexion();
  void ssh_stopConnexion();
  void ssh_autentification();
  void ssh_verify_knownhost();

  void select_fileToSend();

  inline string* get_path_source(){return &path_source;}
  inline string* get_path_target(){return &path_target;}
  inline bool* get_ssh_connected(){return &ssh_connected;}
  inline int* get_ssh_adress_ID(){return &ssh_adress_ID;}

private:
  ssh_session SSH;

  string name_file;
  string path_source;
  string path_target;
  string ssh_adress;
  bool ssh_connected;
  int ssh_adress_ID;
  int ssh_port;
};

#endif
