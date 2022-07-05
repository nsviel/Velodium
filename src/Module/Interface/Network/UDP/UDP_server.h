#ifndef UDP_server_H
#define UDP_server_H

// Server side implementation of UDP client-server model

#include "../../../../common.h"

class Socket;


class UDP_server
{
public:
  //Constructor / Destructor
  UDP_server();
  ~UDP_server();

public:
  //Main functions
  void capture_init(int port, int packet_size);
  void capture_stop();
  vector<int> capture_packet();

private:
  Socket* sock_server;
  Socket* sock_client;
};

#endif
