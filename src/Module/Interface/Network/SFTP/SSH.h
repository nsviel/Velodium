#ifndef PROTOCOL_SSH_H
#define PROTOCOL_SSH_H

#include "../struct_wallet.h"

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
  SSH(Wallet* wallet);
  ~SSH();

public:
  void ssh_startConnexion();
  void ssh_stopConnexion();
  void ssh_autentification();
  void ssh_verify_knownhost();

  inline ssh_session* get_ssh_session(){return &ssh;}
  inline bool* get_ssh_connected(){return &ssh_connected;}
  inline string* get_selected_ip(){return &selected_ip;}
  inline string* get_selected_dest(){return &selected_dest;}
  inline int* get_selected_port(){return &selected_port;}

private:
  ssh_session ssh;
  Wallet* wallet;

  bool ssh_connected;
  string selected_ip;
  string selected_dest;
  int selected_port;
};

#endif
