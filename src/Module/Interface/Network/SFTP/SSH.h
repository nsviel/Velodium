#ifndef PROTOCOL_SSH_H
#define PROTOCOL_SSH_H

#include "../../../../common.h"

#include <libssh/libssh.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <filesystem>
#include <fcntl.h>


class SSH
{
public:
  //Constructor / Destructor
  SSH();
  ~SSH();

public:
  void ssh_startConnexion();
  void ssh_stopConnexion();
  void ssh_autentification();
  void ssh_verify_knownhost();

  inline bool* get_ssh_connected(){return &ssh_connected;}
  inline int* get_ssh_adress_ID(){return &ssh_adress_ID;}
  inline ssh_session* get_ssh_session(){return &ssh;}

private:
  ssh_session ssh;

  string ssh_adress;
  bool ssh_connected;
  int ssh_adress_ID;
  int ssh_port;
};

#endif
