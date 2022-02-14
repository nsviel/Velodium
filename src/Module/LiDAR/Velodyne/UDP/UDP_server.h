#ifndef UDP_SERVER_H
#define UDP_SERVER_H

// Server side implementation of UDP client-server model

#include "../../../../common.h"

class UDP_server
{
public:
  //Constructor / Destructor
  UDP_server();
  ~UDP_server();

public:
  //Main functions
  vector<int> read_UDP_packets();

  //Subfunctions
  void server_binding();
  void server_read_data();

private:
  int PORT;
  int BUF_SIZE;
  int sock;
  bool is_binded;
  string IP_local;

  vector<int> packet_dec;
};

#endif
